/* Nintendo 3DS entry point (docs/PORT.md "Nintendo 3DS"): the port's own main (runtime/main.c, port_main here) with
 * the flags a PC launcher would pass, built from what is on the SD card:
 *   sdmc:/3ds/tombi/            the folder (made if missing; the current directory: saves and settings go here)
 *   sdmc:/3ds/tombi/data/       a folder of tools/extract_data.py, if present; otherwise
 *   sdmc:/3ds/tombi/<image>     the first .chd / .bin / .iso in the folder (the player's own disc image)
 *   sdmc:/3ds/tombi/args.txt    optional extra flags, separated by spaces or lines (e.g. --lang 5, --renderer soft)
 *   sdmc:/3ds/tombi/log.txt     the log (stderr), rewritten on every start
 * Without a disc image the top screen says where to put it and the program waits for START. */
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
static void wait_message(const char *msg) {
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);
    printf("%s\n\nPress START to exit.\n", msg);
    while (aptMainLoop()) {
        hidScanInput();
        if (hidKeysDown() & KEY_START) break;
        gfxFlushBuffers(); gfxSwapBuffers(); gspWaitForVBlank();
    }
    gfxExit();
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    osSetSpeedupEnable(true);                  /* New 3DS: 804 MHz and the L2 cache */
    mkdir("sdmc:/3ds", 0777);
    mkdir(DIR_, 0777);
    chdir(DIR_);
    if (!freopen(DIR_ "/log.txt", "w", stderr)) { /* (no SD card: the log goes nowhere) */ }
    static char image[512], extra[2048];
    static char *args[64];
    int n = 0;
    args[n++] = "tombi";
    struct stat st;
    if (stat(DIR_ "/data/manifest.txt", &st) == 0) { args[n++] = "--data"; args[n++] = DIR_ "/data"; }
    else if (find_image(image, sizeof image)) { args[n++] = "--disc"; args[n++] = image; }
    else {
        wait_message("Tombi! port\n\nNo disc image found.\nCopy your own Tombi! (PAL) disc image\n(.chd, .bin or .iso) to\n\n  " DIR_ "/\n\nand start again.");
        return 0;
    }
    args[n++] = "--renderer"; args[n++] = "pica";   /* the GPU (n3ds/pica_c3d.c); args.txt "--renderer soft": the CPU */
    args[n++] = "--skip-movies";
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
