/* Nintendo 3DS entry point (docs/PORT.md "Nintendo 3DS"): the port's own main (runtime/main.c, port_main here) with
 * the flags a PC launcher would pass, built from what is on the SD card:
 *   sdmc:/3ds/tombi/            the folder (made if missing; the current directory: saves and settings go here)
 *   sdmc:/3ds/tombi/data/       a folder of tools/extract_data.py, if present; otherwise
 *   sdmc:/3ds/tombi/<image>     the first .chd / .bin / .iso in the folder (the player's own disc image)
 *   sdmc:/3ds/tombi/lang.txt    the game's language, 0..8, or 9: Spanish with the improved translation (the chooser
 *                               at the first start writes it; L held: again)
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
/* the face buttons held (runtime/sdl.c: SDL scans the pad every frame): 1 B, 2 A, 4 Y, 8 X */
unsigned port_n3ds_face_buttons(void) {
    u32 k = hidKeysHeld();
    return (k & KEY_B ? 1u : 0) | (k & KEY_A ? 2u : 0) | (k & KEY_Y ? 4u : 0) | (k & KEY_X ? 8u : 0);
}
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
/* a chooser's last act: its text off both screens (the game takes seconds to draw its first frame, and what the
   screens' memory held stayed up meanwhile) */
static void screens_black(void) {
    fprintf(stdout, "\x1b[2J"); fflush(stdout);
    for (int k = 0; k < 2; k++) {
        u16 w, h; u8 *fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, &w, &h);
        if (fb) memset(fb, 0, (size_t)w * h * 3);   /* (gfxInitDefault: 3 bytes a pixel) */
        fb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, &w, &h);
        if (fb) memset(fb, 0, (size_t)w * h * 3);
        gfxFlushBuffers(); gfxSwapBuffers(); gspWaitForVBlank();
    }
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
            if (sel < top) top = sel; if (sel >= top + 20) top = sel - 19;
            /* (fprintf: the program's own printf is the game's, shims/libc.c, which only logs) */
            fprintf(stdout, "\x1b[2J\x1b[1;1HTombi! port - no disc image found\n"
                            "Copy your Tombi! (PAL) disc image (.bin, .iso or\n.chd) to the SD card, folder /3ds/tombi/\n"
                            "or choose it here:  %.28s\n\n", cur);
            for (int i = top; i < nent && i < top + 20; i++) fprintf(stdout, "%c %.44s%s\n", i == sel ? '>' : ' ', ent[i].name, ent[i].dir ? "/" : "");
            if (!nent) fprintf(stdout, " (no folders or .chd / .bin / .iso here)\n");
            fprintf(stdout, "\x1b[28;1HA: open / choose   B: back   START: exit");
            fflush(stdout);
            redraw = 0;
        }
        gfxFlushBuffers(); gfxSwapBuffers(); gspWaitForVBlank();
    }
    screens_black();
    gfxExit();
    return ok;
}

/* The game's language: the disc's own boot menu (a program of its own, which the port does not run) offers five of
   the nine languages every PAL disc carries; this one offers them all.  Shown the first time and when L is held at
   start; the choice is kept in lang.txt.  Returns 0..8 (--lang) or 9, Spanish with the improved translation (the
   patch the program carries: port_es_tpatch), -1: START (leave). */
#define NLANG 10
static int choose_lang(int code) {
    /* (the console's font is the PC's code page 437: its codes for the accented letters) */
    static const char *name[NLANG] = { "English", "Fran\x87" "ais", "Deutsch", "Italiano", "Nederlands", "Espa\xA4ol (traducci\xA2n mejorada)",
                                       "Espa\xA4ol (original)", "Portugues", "Svenska", "Dansk" };
    static const int codes[NLANG] = { 0, 1, 2, 3, 4, 9, 5, 6, 7, 8 };
    int redraw = 1, ok = 0, sel = 0;
    for (int i = 0; i < NLANG; i++) if (codes[i] == code) sel = i;
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);
    while (aptMainLoop()) {
        hidScanInput();
        u32 k = hidKeysDown() | (hidKeysDownRepeat() & (KEY_UP | KEY_DOWN));
        if (k & KEY_START) break;
        if ((k & KEY_DOWN) && sel < NLANG - 1) { sel++; redraw = 1; }
        if ((k & KEY_UP) && sel > 0) { sel--; redraw = 1; }
        if (k & KEY_A) { ok = 1; break; }
        if (redraw) {
            fprintf(stdout, "\x1b[2J\x1b[1;1HTombi! port - language\n\n");
            for (int i = 0; i < NLANG; i++) fprintf(stdout, "%c %s\n", i == sel ? '>' : ' ', name[i]);
            fprintf(stdout, "\n(hold L while the game starts to choose again)\n");
            fprintf(stdout, "\nLa traducci\xA2n mejorada se prepara la primera vez:\nunos minutos, con una barra de progreso.\n");
            fprintf(stdout, "\x1b[28;1HA: choose   START: exit");
            fflush(stdout);
            redraw = 0;
        }
        gfxFlushBuffers(); gfxSwapBuffers(); gspWaitForVBlank();
    }
    screens_black();
    gfxExit();
    return ok ? codes[sel] : -1;
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
    {   /* the language: lang.txt, asked the first time and with L held (an args.txt "--lang N" comes later and wins) */
        static char langarg[4];
        extern const unsigned char port_es_tpatch[]; extern const unsigned port_es_tpatch_size;
        int lang = -1, ask, given = 0;
        svcSleepThread(50000000); hidScanInput(); ask = (hidKeysHeld() & KEY_L) != 0;
        FILE *lf = fopen(DIR_ "/args.txt", "r");   /* (a language given there: nothing to ask) */
        if (lf) { static char t[2048]; size_t len = fread(t, 1, sizeof t - 1, lf); t[len] = 0; fclose(lf); given = strstr(t, "--lang") != 0; }
        lf = given ? 0 : fopen(DIR_ "/lang.txt", "r");
        if (lf) { if (fscanf(lf, "%d", &lang) != 1 || lang < 0 || lang > 9) lang = -1; fclose(lf); }
        if (!given && (lang < 0 || ask)) {
            lang = choose_lang(lang < 0 ? 0 : lang);
            if (lang < 0) return 0;
            lf = fopen(DIR_ "/lang.txt", "w");
            if (lf) { fprintf(lf, "%d\n", lang); fclose(lf); }
        }
        if (lang == 9) {   /* Spanish, the improved translation: its patch written once next to the disc image */
            struct stat ps;
            if (stat(DIR_ "/es_mejorada.tpatch", &ps) != 0 || (unsigned)ps.st_size != port_es_tpatch_size) {
                FILE *pf = fopen(DIR_ "/es_mejorada.tpatch", "wb");
                if (pf) { fwrite(port_es_tpatch, 1, port_es_tpatch_size, pf); fclose(pf); }
            }
            /* (the files it rebuilds are kept on the SD card, made at the first start: shims/tpatch.c port_tpatch_store) */
            { extern const char *port_tpatch_store; port_tpatch_store = DIR_ "/es_mejorada.cache"; }
            args[n++] = "--lang"; args[n++] = "5";
            if (stat(DIR_ "/es_mejorada.tpatch", &ps) == 0) { args[n++] = "--text-patch"; args[n++] = DIR_ "/es_mejorada.tpatch"; }
        } else if (lang > 0) { snprintf(langarg, sizeof langarg, "%d", lang); args[n++] = "--lang"; args[n++] = langarg; }
    }
    args[n++] = "--renderer"; args[n++] = "pica";   /* the GPU (n3ds/pica_c3d.c); args.txt "--renderer soft": the CPU */
    { bool n3 = false; APT_CheckNew3DS(&n3);   /* (an Old 3DS decodes a movie at 3 frames a second: none there) */
      /* (the movies play on both, decoded by the fast transform, shims/mdec.c --mdec-fast; an Old 3DS decoded three
         frames a second with the reference one and had none.  args.txt "--skip-movies": none) */
      args[n++] = "--mdec-fast";
      if (n3) args[n++] = "--logo-only";
      if (!n3) { args[n++] = "--audio-half"; args[n++] = "--audio-dry"; args[n++] = "--lazy-clut"; args[n++] = "2"; }          /* (its second core cannot mix 44100 samples a second: 22050 there) */
      args[n++] = "--frameskip"; }                  /* (behind its clock, the port draws fewer frames, the game keeps its speed) */   /* (New 3DS: the developer's logo plays, a 6.7 s movie, the others are skipped) */
    /* 60 Hz: the 3DS's screens refresh at ~60 Hz (50 on them repeats every fifth frame), and it is the US / NTSC game
       speed (the PAL build kept the US logic, docs/PORT.md "60 Hz mode"); args.txt "--hz 50" for the PAL speed */
    /* (an Old 3DS: 50 Hz, the PAL disc's own speed: its 268 MHz cannot run 60 game frames a second, and at 50 the
       frame's 20 ms hold the game, the sound and a drawn frame in four) */
    { bool n3 = false; APT_CheckNew3DS(&n3); args[n++] = "--hz"; args[n++] = n3 ? "60" : "50"; }
    /* the bottom screen: the panel (runtime/panel.c) at the screen's own size, drawn by n3ds/pica_c3d.c; the game's
       NOW LOADING until gameplay, then status / events / map / items, by touch */
    args[n++] = "--second-screen"; args[n++] = "--panel-size"; args[n++] = "320x240";
    /* (an Old 3DS: without the map tab, whose tiles are more than it has time for; the other tabs stay) */
    { bool n3 = false; APT_CheckNew3DS(&n3); if (!n3) args[n++] = "--panel-no-map"; }
    args[n++] = "--log-fps"; args[n++] = "--prompt-big"; args[n++] = "6";   /* (the button prompts: a round black button lit from the upper left, the letter white: shims/prompts.c) */                        /* a line a second in log.txt: where a frame's time goes on this console */
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
