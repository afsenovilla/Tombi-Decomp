/* libmain.so: the SDL_main of the Android build (docs/PORT.md "4. Android").
 *
 * The game (libtombi.so) keeps the PC port's phase 1 pointer model: 32-bit pointers, and every game datum below
 * 16 MB so the 24-bit ordering-table links are exact (port_prelude.h, port_check_layout).  A shared library is
 * normally mapped wherever the dynamic linker likes, so this loader
 *   1. reserves [PORT2_LOW_BASE, +PORT2_LOW_SIZE) with mmap as early as possible (JNI_OnLoad, when SDLActivity
 *      loads libmain.so, before SDL's threads and the GL driver map anything there);
 *   2. in SDL_main, loads libtombi.so into that reservation with android_dlopen_ext(ANDROID_DLEXT_RESERVED_ADDRESS);
 *   3. finds the game data: the folder tools/extract_data.py / the launcher prepares (the PC port's --data folder:
 *      data/manifest.txt, EXE/MAIN0.EXE, AREAxx/...), in the app's external files dir
 *      (Android/data/<package>/files/data).  No game data is in the APK.  Missing: a message box says where to put it;
 *   4. calls port_main with PC-style flags: --data, --save-dir, --fullscreen, --renderer soft (the GLES path of
 *      shims/gpu_gl.c is untested here), then the words of files/args.txt (any PC flag, e.g. "--lang 5 --fps60").
 * stderr (the port's log) goes to files/tombi.log. */
#include <android/dlext.h>
#include <android/log.h>
#include <dlfcn.h>
#include <errno.h>
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#define PORT2_LOW_BASE 0x00100000ul
#define PORT2_LOW_SIZE 0x02000000ul      /* 32 MB: libtombi.so is ~19 MB with its .bss; port_ram must end below 16 MB */
#define TAG "tombi"

static void *low_base;
static char low_err[160];

static void reserve_low(void) {
    if (low_base) return;
    void *p = mmap((void *)PORT2_LOW_BASE, PORT2_LOW_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
    if (p == MAP_FAILED) { snprintf(low_err, sizeof low_err, "mmap: %s", strerror(errno)); return; }
    if (p != (void *)PORT2_LOW_BASE) {      /* only a hint without MAP_FIXED (which would clobber a mapping there) */
        snprintf(low_err, sizeof low_err, "the range at %#lx is in use (got %p)", PORT2_LOW_BASE, p);
        munmap(p, PORT2_LOW_SIZE);
        return;
    }
    low_base = p;
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    (void)vm; (void)reserved;
    reserve_low();
    return JNI_VERSION_1_4;
}

static int fail(const char *title, const char *msg) {
    __android_log_print(ANDROID_LOG_ERROR, TAG, "%s: %s", title, msg);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, msg, NULL);
    return 1;
}

int SDL_main(int argc, char *argv[]) {
    static char msg[1024], data[512], saves[512], logp[512], argsbuf[2048];
    char *args[64]; int n = 0;
    (void)argc;
    const char *files = SDL_AndroidGetExternalStoragePath();     /* Android/data/<package>/files (created by SDL) */
    if (!files) return fail("Tombi!", "No app storage folder (SDL_AndroidGetExternalStoragePath failed).");
    snprintf(data, sizeof data, "%s/data", files);
    snprintf(saves, sizeof saves, "%s/saves", files);
    snprintf(logp, sizeof logp, "%s/tombi.log", files);
    mkdir(saves, 0755);
    if (chdir(files) != 0) __android_log_print(ANDROID_LOG_WARN, TAG, "chdir %s: %s", files, strerror(errno));
    {
        char man[600]; struct stat st;
        snprintf(man, sizeof man, "%s/manifest.txt", data);
        if (stat(man, &st) != 0) {
            snprintf(msg, sizeof msg,
                     "Game data not found.\n\nCopy the data folder prepared from your own Tombi! disc (the same folder the PC port "
                     "uses with --data: made by the launcher or tools/extract_data.py; it holds manifest.txt) to:\n\n%s\n\n"
                     "The app ships no game data.", data);
            return fail("Tombi! - game data missing", msg);
        }
    }
    reserve_low();
    if (!low_base) {
        snprintf(msg, sizeof msg, "Could not reserve the low address range the game needs (%s).", low_err);
        return fail("Tombi!", msg);
    }
    android_dlextinfo ext; memset(&ext, 0, sizeof ext);
    ext.flags = ANDROID_DLEXT_RESERVED_ADDRESS;
    ext.reserved_addr = low_base;
    ext.reserved_size = PORT2_LOW_SIZE;
    void *h = android_dlopen_ext("libtombi.so", RTLD_NOW | RTLD_LOCAL, &ext);
    if (!h) { snprintf(msg, sizeof msg, "Loading libtombi.so failed: %s", dlerror()); return fail("Tombi!", msg); }
    int (*port_main)(int, char **) = (int (*)(int, char **))dlsym(h, "port_main");
    if (!port_main) return fail("Tombi!", "libtombi.so has no port_main.");
    if (!freopen(logp, "w", stderr)) __android_log_print(ANDROID_LOG_WARN, TAG, "cannot write %s", logp);
    args[n++] = argv && argv[0] ? argv[0] : (char *)"tombi";
    args[n++] = (char *)"--data"; args[n++] = data;
    args[n++] = (char *)"--save-dir"; args[n++] = saves;
    args[n++] = (char *)"--fullscreen";
    args[n++] = (char *)"--renderer"; args[n++] = (char *)"soft";
    args[n++] = (char *)"--widescreen"; args[n++] = (char *)"auto";   /* fit the panel (args.txt "--aspect 4:3" turns it off) */
    {   /* files/args.txt: extra PC flags, whitespace separated (later flags override the defaults above) */
        char ap[600]; snprintf(ap, sizeof ap, "%s/args.txt", files);
        FILE *f = fopen(ap, "r");
        if (f) {
            size_t len = fread(argsbuf, 1, sizeof argsbuf - 1, f); fclose(f);
            argsbuf[len] = 0;
            for (char *t = strtok(argsbuf, " \t\r\n"); t && n < 63; t = strtok(0, " \t\r\n")) args[n++] = t;
        }
    }
    args[n] = 0;
    __android_log_print(ANDROID_LOG_INFO, TAG, "libtombi.so at %p, %d args, log in %s", low_base, n, logp);
    return port_main(n, args);
}
