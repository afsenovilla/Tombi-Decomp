/* Nintendo 3DS entry point (docs/PORT.md "Nintendo 3DS"): the port's own main (runtime/main.c, port_main here) with
 * the flags a PC launcher would pass, built from what is on the SD card:
 *   sdmc:/3ds/tombi/            the folder (made if missing; the current directory: saves and settings go here)
 *   sdmc:/3ds/tombi/data/       a folder of tools/extract_data.py, if present; otherwise
 *   sdmc:/3ds/tombi/<image>     the first .chd / .bin / .iso in the folder (the player's own disc image)
 *   sdmc:/3ds/tombi/args.txt    optional extra flags, separated by spaces or lines (e.g. --lang 5, --renderer soft)
 *   sdmc:/3ds/tombi/log.txt     the log (stderr), rewritten on every start
 *   sdmc:/3ds/tombi/disc.txt    the image chosen in the chooser, anywhere on the card
 * Without an image there, the top screen lists the card's folders to choose one from (SELECT held at the start: always). */
#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

u32 __stacksize__ = 1024 * 1024;   /* the main thread runs the game's main loop (libctru's default is 32 KB) */

int port_main(int argc, char **argv);

/* the port's clock (runtime/main.c on the other systems): the ARM11 system tick, 268 MHz */
unsigned long long port_time_us(void) { return svcGetSystemTick() / (SYSCLOCK_ARM11 / 1000000); }
void port_sleep_us(unsigned long long us) { svcSleepThread((s64)us * 1000); }

#define DIR_ "sdmc:/3ds/tombi"

static int has_ext(const char *n, const char *e) {
    size_t a = strlen(n), b = strlen(e);
    return a > b && !strcasecmp(n + a - b, e);
}
static int find_image(char *out, size_t cap) {
    static const char *const exts[] = { ".chd", ".bin", ".iso" };
    for (int k = 0; k < 3; k++) {
        DIR *d = opendir(DIR_);
        if (!d) return 0;
        struct dirent *e;
        while ((e = readdir(d))) if (has_ext(e->d_name, exts[k])) { snprintf(out, cap, DIR_ "/%s", e->d_name); closedir(d); return 1; }
        closedir(d);
    }
    return 0;
}
/* ---- the disc image chooser: a list of the SD card's folders and disc images on the top screen.  Shown when no image
   is in the port's folder and none was chosen before (the choice is kept in disc.txt), or with SELECT held at the
   start.  In an emulator the "SD card" is the emulator's own sdmc folder: the image only has to be somewhere in it. */
#define MAX_ENT 400
typedef struct { char name[96]; int dir; } Ent;
static Ent ent[MAX_ENT];
static int nent;
static int ent_cmp(const void *a, const void *b) {   /* folders first, then by name */
    const Ent *x = a, *y = b;
    return x->dir != y->dir ? y->dir - x->dir : strcasecmp(x->name, y->name);
}
static void list_dir(const char *path) {
    nent = 0;
    DIR *d = opendir(path);
    if (!d) return;
    struct dirent *e;
    while ((e = readdir(d)) && nent < MAX_ENT) {
        if (e->d_name[0] == '.' || strlen(e->d_name) >= sizeof ent[0].name) continue;
        int dir = e->d_type == DT_DIR;
        if (!dir && !has_ext(e->d_name, ".chd") && !has_ext(e->d_name, ".bin") && !has_ext(e->d_name, ".iso") && !has_ext(e->d_name, ".img")) continue;
        strcpy(ent[nent].name, e->d_name); ent[nent].dir = dir; nent++;
    }
    closedir(d);
    qsort(ent, (size_t)nent, sizeof ent[0], ent_cmp);
}
static int choose_image(char *out, size_t cap) {
    static char cur[400] = "sdmc:/";
    int sel = 0, top = 0, redraw = 1, ok = 0;
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);
    list_dir(cur);
    while (aptMainLoop()) {
        hidScanInput();
        u32 k = hidKeysDown() | (hidKeysDownRepeat() & (KEY_UP | KEY_DOWN));
        if (k & KEY_START) break;
        if ((k & KEY_DOWN) && sel + 1 < nent) { sel++; redraw = 1; }
        if ((k & KEY_UP) && sel > 0) { sel--; redraw = 1; }
        if (k & KEY_RIGHT) { sel = sel + 20 < nent ? sel + 20 : nent ? nent - 1 : 0; redraw = 1; }
        if (k & KEY_LEFT) { sel = sel > 20 ? sel - 20 : 0; redraw = 1; }
        if ((k & KEY_B) && strlen(cur) > 6) {   /* up a folder */
            char *q = cur + strlen(cur) - 1; *q = 0; q = strrchr(cur, '/'); q[1] = 0;
            list_dir(cur); sel = top = 0; redraw = 1;
        }
        if ((k & KEY_A) && nent) {
            if (ent[sel].dir) {
                if (strlen(cur) + strlen(ent[sel].name) + 2 < sizeof cur) { strcat(cur, ent[sel].name); strcat(cur, "/"); list_dir(cur); sel = top = 0; redraw = 1; }
            } else { snprintf(out, cap, "%s%s", cur, ent[sel].name); ok = 1; break; }
        }
        if (redraw) {
            if (sel < top) top = sel; if (sel >= top + 22) top = sel - 21;
            /* (fprintf: the program's own printf is the game's, shims/libc.c, which only logs) */
            fprintf(stdout, "\x1b[2J\x1b[1;1HTombi! port - choose your disc image\n%.48s\n\n", cur);
            for (int i = top; i < nent && i < top + 22; i++) fprintf(stdout, "%c %.44s%s\n", i == sel ? '>' : ' ', ent[i].name, ent[i].dir ? "/" : "");
            if (!nent) fprintf(stdout, " (no folders or .chd / .bin / .iso here)\n");
            fprintf(stdout, "\x1b[28;1HA: open / choose   B: back   START: exit");
            fflush(stdout);
            redraw = 0;
        }
        gfxFlushBuffers(); gfxSwapBuffers(); gspWaitForVBlank();
    }
    gfxExit();
    return ok;
}

/* Sound in an emulator: libctru's ndsp (SDL's audio) loads the DSP firmware from sdmc:/3ds/dspfirm.cdc, a file a console
   that runs homebrew has and an emulator's SD folder does not.  Azahar / Citra emulate the DSP themselves (HLE) and do
   not run the firmware: there, without the file, ndsp is given an empty component (a DSP1 header with no segments).
   The emulator is told apart by its own svcGetSystemInfo type (0x20000: an error on a console), so a console without
   the file still reports it missing instead of waiting for a DSP that never answers. */
static void dsp_component_for_emulators(void) {
    struct stat st;
    if (stat("sdmc:/3ds/dspfirm.cdc", &st) == 0) return;
    s64 is_emu = 0;
    if (R_FAILED(svcGetSystemInfo(&is_emu, 0x20000, 0)) || !is_emu) return;
    static u8 comp[0x300];
    memcpy(comp + 0x100, "DSP1", 4);
    comp[0x104] = 0x00; comp[0x105] = 0x03;   /* (the size: the header alone) */
    ndspUseComponent(comp, sizeof comp, 0xFF, 0xFF);
    fprintf(stderr, "[3ds] no dspfirm.cdc, an emulator: ndsp gets an empty DSP component\n");
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    osSetSpeedupEnable(true);                  /* New 3DS: 804 MHz and the L2 cache */
    mkdir("sdmc:/3ds", 0777);
    mkdir(DIR_, 0777);
    chdir(DIR_);
    if (!freopen(DIR_ "/log.txt", "w", stderr)) { /* (no SD card: the log goes nowhere) */ }
    dsp_component_for_emulators();
    static char image[512], extra[2048];
    static char *args[64];
    int n = 0;
    args[n++] = "tombi";
    struct stat st;
    if (stat(DIR_ "/data/manifest.txt", &st) == 0) { args[n++] = "--data"; args[n++] = DIR_ "/data"; }
    else {
        /* the image chosen before (disc.txt), else one in the port's folder, else the chooser; SELECT held: the chooser */
        int have = 0, ask = 0;
        svcSleepThread(50000000); hidScanInput(); ask = (hidKeysHeld() & KEY_SELECT) != 0;
        FILE *df = ask ? 0 : fopen(DIR_ "/disc.txt", "r");
        if (df) {
            if (fgets(image, sizeof image, df)) { image[strcspn(image, "\r\n")] = 0; have = image[0] && stat(image, &st) == 0; }
            fclose(df);
        }
        if (!have && !ask) have = find_image(image, sizeof image);
        if (!have) {
            if (!choose_image(image, sizeof image)) return 0;
            df = fopen(DIR_ "/disc.txt", "w");
            if (df) { fprintf(df, "%s\n", image); fclose(df); }
        }
        args[n++] = "--disc"; args[n++] = image;
    }
    args[n++] = "--renderer"; args[n++] = "pica";   /* the GPU (n3ds/pica_c3d.c); args.txt "--renderer soft": the CPU */
    args[n++] = "--skip-movies";
    /* 60 Hz: the 3DS's screens refresh at ~60 Hz (50 on them repeats every fifth frame), and it is the US / NTSC game
       speed (the PAL build kept the US logic, docs/PORT.md "60 Hz mode"); args.txt "--hz 50" for the PAL speed */
    args[n++] = "--hz"; args[n++] = "60";
    /* the bottom screen: the panel (runtime/panel.c) at the screen's own size, drawn by n3ds/pica_c3d.c; the game's
       NOW LOADING until gameplay, then status / events / map / items, by touch */
    args[n++] = "--second-screen"; args[n++] = "--panel-size"; args[n++] = "320x240";
    { extern int port_panel_canvas_out; port_panel_canvas_out = 1; }
    FILE *f = fopen(DIR_ "/args.txt", "r");
    if (f) {
        size_t len = fread(extra, 1, sizeof extra - 1, f); extra[len] = 0; fclose(f);
        for (char *p = strtok(extra, " \t\r\n"); p && n < 62; p = strtok(0, " \t\r\n")) args[n++] = p;
    }
    args[n] = 0;
    fprintf(stderr, "[3ds] port_main:");
    for (int i = 1; i < n; i++) fprintf(stderr, " %s", args[i]);
    fprintf(stderr, "\n");
    return port_main(n, args);
}
