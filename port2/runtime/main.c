/* Entry of the native-pointer port: command line, host helpers, and the game's main() in C.
 *
 * main() (0x800163F4, 1396 bytes) is the one game function the matching decomp keeps as Psy-Q-classified asm, so
 * the port carries this C translation of it (port/runtime/main_port.c, from
 * build/full/asm/main0/nonmatchings/main0/main.s).  The only changes: the vblank busy-wait calls port_vblank()
 * (there is no interrupt to advance the counter), and the frame limit / exit is handled by the runtime.  The
 * scratchpad is the port_scratch array of the data image. */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "port_runtime.h"

typedef unsigned char u8; typedef unsigned short u16; typedef short s16; typedef unsigned int u32;

int port_verbose;

/* The log goes to stderr, unbuffered (setvbuf below, and an fflush after every write in case a CRT ignores it),
   so a hard kill loses nothing.  The watchdog's report must leave the process even when the game thread sits
   inside an fprintf (suspended there on Windows, or interrupted there by SIGUSR1 on Linux) holding the stream's
   lock: while port_log_direct is set, or from the thread port_log_direct_tid (Win32), the line is formatted into a
   local buffer and written with the raw write / WriteFile of port_log_write_raw (host_win32.c on Windows). */
volatile int port_log_direct;
unsigned long port_log_direct_tid;
#ifndef _WIN32
#include <unistd.h>
#include <sys/syscall.h>
static int log_is_direct(void) { return port_log_direct > 0; }
/* (the raw syscall: `write` is the game's Psy-Q stub in this binary, gen/stubs.c, not libc's) */
static void port_log_write_raw(const char *s, int n) { while (n > 0) { long w = syscall(SYS_write, 2, s, n); if (w <= 0) break; s += w; n -= w; } }
#else
int port_log_is_direct(void); void port_log_write_raw(const char *s, int n);
#define log_is_direct port_log_is_direct
#endif
void port_printf(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    if (log_is_direct()) {
        char buf[2048]; int n = vsnprintf(buf, sizeof buf, fmt, ap);
        if (n > (int)sizeof buf - 1) n = sizeof buf - 1;
        if (n > 0) port_log_write_raw(buf, n);
    } else { vfprintf(stderr, fmt, ap); fflush(stderr); }
    va_end(ap);
}
void port_fatal(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt); fputs("[port2] FATAL: ", stderr); vfprintf(stderr, fmt, ap); va_end(ap);
    fputc('\n', stderr);
    exit(1);
}
static int stub_count;
static const char *stub_names[64]; static int stub_hits[64]; static int stub_n;
void port_stub_hit(const char *name) {
    stub_count++;
    for (int i = 0; i < stub_n; i++) if (stub_names[i] == name) { stub_hits[i]++; return; }
    if (stub_n < 64) { stub_names[stub_n] = name; stub_hits[stub_n++] = 1; }
    if (stub_n <= 64) fprintf(stderr, "[port2] stub: %s (frame %d)\n", name, port_frame);
}
void port_stub_report(void) {
    extern int port_missing_calls(void);
    if (!stub_count && !port_missing_calls()) { port_printf("[port2] no stub was called\n"); return; }
    port_printf("[port2] %d stub calls (%d distinct), %d calls into the overlay region without native code:", stub_count, stub_n, port_missing_calls());
    for (int i = 0; i < stub_n; i++) port_printf(" %s=%d", stub_names[i], stub_hits[i]);
    port_printf("\n");
    if (port_exec_from_ram_calls()) port_printf("[port2] %d calls into RAM dispatched through the fault handler\n", port_exec_from_ram_calls());
}
void port_flush_log(void) { fflush(stderr); }
void port_quit(int code) {
    port_gpu_stop();
    port_window_stop();
    exit(code);
}

#ifndef _WIN32    /* (Windows: runtime/host_win32.c) */
unsigned long long port_time_us(void) {
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
    return (unsigned long long)ts.tv_sec * 1000000ull + ts.tv_nsec / 1000;
}
void port_sleep_us(unsigned long long us) {
    struct timespec ts = { us / 1000000ull, (us % 1000000ull) * 1000 };
    while (nanosleep(&ts, &ts) && errno == EINTR) ;
}
#endif
void port_sleep_until_us(unsigned long long deadline) {
    unsigned long long now = port_time_us();
    if (deadline > now) port_sleep_us(deadline - now);
}

/* Phase 1 keeps the ordering-table links as 24-bit addresses, which is exact only while every game datum sits
   below 16 MB (see port_otaddr in port_prelude.h). */
int port_check_layout(void) {
    return (unsigned long)port_ram + 0x200000 < 0x1000000ul && (unsigned long)port_scratch + 0x400 < 0x1000000ul;
}

/* ------------------------------------------------------------------ the game's main() */
extern void __main(void);
extern void SetDispMask(int);
extern int SetVideoMode(int);
extern int ResetCallback(void);
extern void initGlobalState(void);
extern int ResetGraph(int);
extern int SetGraphDebug(int);
extern void InitGeom(void);
extern int CdInit(void);
extern int CdControl(u8, u8 *, u8 *);
extern void DecDCTReset(int);
extern void initGraphics(void);
extern void setDrawSyncParams(int, int);
extern void SoundInit(void);
extern void *SetDefDispEnv(void *, int, int, int, int);
extern void MemCardInit(void);
extern void joypadInit(void);
extern void initTasks(void);
extern void startTask(int, void *);
extern void func_80019244(void);
extern int EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern int OpenEvent(u32, int, int, void *);
extern int EnableEvent(int);
extern void vblankHandler(void);
extern void ThreadTableRun(void);
extern int DrawSync(int);
extern void FlipFrame(void);
extern void tickTaskTimers(void);
extern int VSync(int);
extern void *PutDispEnv(void *);
extern u8 D_8009BC80[];          /* DISPENV */
extern u16 D_8009F7F2;           /* pad 2 buttons (active low): debug screen offset */
extern u8 D_800A2528[];
extern void *D_8009D540;

static void game_main(void) {
    u8 param[8];
    u16 *dispx, *dispy;

    __main();
    SetDispMask(0);
    SetVideoMode(1);
    ResetCallback();
    initGlobalState();
    ResetGraph(0);
    SetGraphDebug(0);
    InitGeom();
    CdInit();
    param[0] = 0x80;
    while (CdControl(14 /* CdlSetmode */, param, 0) == 0) {}
    DecDCTReset(0);
    SPAD32(0x2A0) = 0;
    SPAD32(0x29C) = 0;
    initGraphics();
    setDrawSyncParams(1, 1);
    SoundInit();
    SetDefDispEnv(D_8009BC80, 0, 0, 0x200, 0x100);
    dispx = (u16 *)D_8009BC80;            /* disp.x / disp.y: moved with pad 2 in debug mode */
    dispy = (u16 *)(D_8009BC80 + 2);
    MemCardInit();
    joypadInit();
    initTasks();
    startTask(0, (void *)func_80019244);
    EnterCriticalSection();
    SPAD32(0x1D8) = OpenEvent(0xF2000003u, 2, 0x1000, (void *)vblankHandler);
    ExitCriticalSection();
    EnableEvent(SPAD32(0x1D8));
    SetDispMask(1);
    port_printf("[port2] game main: init done, entering the main loop\n");

    for (;;) {
        SPAD16(0x1E8) = 0;
        if (SPAD16(0x1F0) <= 0x4000) {
            D_8009D540 = D_800A2528 + (s16)SPAD16(0x1F4) * 0x780;
            ThreadTableRun();
        }
        if (SPAD16(0x1EC)) DrawSync(0);
        while (SPAD16(0x1E8) < SPAD16(0x1EA)) port_vblank();     /* frame pacing: vblank counter */
        if (!SPAD16(0x1EC)) ResetGraph(1);
        if (SPAD8(0x1BD) && (SPAD8(0x1D1) | SPAD8(0x1D0))) {
            while (~D_8009F7F2 & 0x200) {                          /* debug: pad 2 moves the display */
                if (~D_8009F7F2 & 0x80) *dispx -= 4;
                if (~D_8009F7F2 & 0x20) *dispx += 4;
                if (~D_8009F7F2 & 0x10) *dispy -= 4;
                if (~D_8009F7F2 & 0x40) *dispy += 4;
                VSync(0);
                PutDispEnv(D_8009BC80);
            }
        }
        {
            int m = (signed char)SPAD8(0x1CC);
            int flip = 0;
            if (m == 3) { SPAD8(0x1CC) = 2; flip = SPAD16(0x1F0) <= 0x4000; }
            else if (m >= 0 && m < 2) flip = SPAD16(0x1F0) <= 0x4000;
            if (flip) { FlipFrame(); tickTaskTimers(); }
        }
        if (SPAD8(0x1BF)) {
            if (!SPAD8(0x1CC) && (SPAD8(0x1D1) | SPAD8(0x1D0)) && SPAD8(0x1CE) == 1 && (SPAD16(0x1FC) & 0x400))
                SPAD16(0x1F0) = 0x8000 - SPAD16(0x1F0);
        } else if (SPAD8(0x1BE) && !SPAD8(0x1CC) && (SPAD8(0x1D1) | SPAD8(0x1D0)) && SPAD8(0x1CE) == 1) {
            u16 pad = SPAD16(0x1FC);
            if (pad & 0x400) {
                SPAD16(0x1EE) = 1 - SPAD16(0x1EE);
                SPAD16(0x1F0) = SPAD16(0x1EE) << 15;
            }
            if (SPAD16(0x1EE)) {
                SPAD16(0x1F0) = 0x8000;
                if (pad & 0x100) SPAD16(0x1F0) = 0;
            } else if (pad & 0x100) {
                u16 v = SPAD16(0x1F0);
                v = (~v & 0x4000) | (v & 0xBFFF);
                SPAD16(0x1F0) = v;
                if (!(v & 0x4000)) SPAD16(0x1F0) = v & 0xFFF0;
            } else if (SPAD16(0x1F0) & 0x4000) {
                SPAD16(0x1F0) = (SPAD16(0x1F0) + 1) & 0xFFF3;
            }
        }
    }
}

/* ------------------------------------------------------------------ command line */
const char *port_disc_path = 0;         /* --disc: the disc image (default game/disc/Tombi.bin when there is no data/) */
const char *port_data_dir = 0;          /* --data: the folder of tools/extract_data.py (default data/ next to the exe, or in the cwd) */
/* the game data: --data DIR, else --disc IMAGE, else data/manifest.txt in the cwd or next to the executable, else
   the default image path (docs/PORT.md "Running without the disc image") */
static void open_game_data(const char *argv0) {
    static char buf[1200];
    if (port_data_dir) {
        if (port_cd_open_data(port_data_dir) < 0) port_fatal("no data/manifest.txt in %s (run tools/extract_data.py on your disc image first)", port_data_dir);
        return;
    }
    if (port_disc_path) {
        if (port_cd_open(port_disc_path) < 0) port_printf("[port2] warning: no disc image at %s: CD reads will fail\n", port_disc_path);
        return;
    }
    if (port_cd_open_data("data") == 0) { port_data_dir = "data"; return; }
    {   /* next to the executable (argv[0] carries the directory when the shell did not search PATH) */
        int n = (int)strlen(argv0);
        while (n > 0 && argv0[n - 1] != '/' && argv0[n - 1] != '\\') n--;
        if (n > 0 && n < (int)sizeof buf - 8) { memcpy(buf, argv0, n); strcpy(buf + n, "data"); if (port_cd_open_data(buf) == 0) { port_data_dir = buf; return; } }
    }
    port_disc_path = "game/disc/Tombi.bin";
    if (port_cd_open(port_disc_path) < 0)
        port_printf("[port2] warning: no data/ folder (tools/extract_data.py) and no disc image at %s: CD reads will fail\n", port_disc_path);
}
static int usage(const char *argv0) {
    port_printf("usage: %s [--data DIR | --disc Tombi.bin|.cue|.iso] [--lang N] [--frames N] [--headless] [--window [--scale N] [--aspect 4:3|square|16:9|21:9|32:9] [--widescreen [16:9|21:9|32:9]] [--fullscreen] [--keys up=Up,cross=X,...]]\n"
                "       [--renderer gl|soft] [--internal-res 1..4] [--fps60] [--realtime] [--hz 50|60] [--show-fps [1|2]] [--patch-tex FILE]... [--title-bg FILE]\n"
                "       [--dump-frames DIR [--dump-every N] [--dump-vram]] [--dump-draws FILE] [--dump-textures DIR] [--texture-pack DIR] [--pad-script FRAME:BUTTONS[:LEN[xCOUNT/PERIOD]],...]\n"
                "       [--poke FRAME:ADDR:VALUE[:SIZE[xCOUNT]]] [--poke-every INTERVAL:ADDR:VALUE[:SIZE]] [--menu FRAME:N] [--title FRAME:ID] [--movie FRAME:ID]\n"
                "       [--dump-audio PATH] [--skip-movies] [--skip-intro] [--trace-cd] [--save-dir DIR] [-v]\n"
                "  --lang N plays in the language of EXE/MAINn.EXE: 0 English (default), 5 Espanol, 6 Portugues, 7 Svenska, 8 Dansk\n"
                "  (1..4 the other MAINn of the disc, docs/PORT.md \"Languages\")\n"
                "  --data DIR reads the game files extracted once from your disc by tools/extract_data.py (default: data/ in the\n"
                "  current folder or next to the executable); --disc reads the disc image itself\n"
                "  the default is a window (SDL2) paced at 50 Hz; --headless runs as fast as possible without one\n"
                "  --renderer gl (default with a window) draws with OpenGL, soft with the software rasteriser (headless default)\n"
                "  --internal-res N (OpenGL) renders at N x the PS1 resolution (1 = original); --dump-frames also writes hd_*.png\n"
                "  --fps60 (OpenGL, experimental) presents frames interpolated between the 50 Hz logic steps\n"
                "  --dump-textures DIR writes every texture drawn as PNG; --texture-pack DIR (OpenGL) draws the PNGs of a pack\n"
                "  instead (any integer scale; docs/PORT.md \"Texture packs\")\n", argv0);
    return 2;
}
int main(int argc, char **argv) {
    extern void port_pad_script(const char *), port_poke_add(const char *), port_poke_add_every(const char *, int),
        port_menu_add(const char *), port_title_add(const char *), port_dump_ram_add(const char *);
    extern const char *port_audio_path, *port_save_dir; extern int port_skip_movies, port_show_fps, port_stall_frame, port_stall_ms;
    int headless = 0, test_null = 0;
    port_window = 1;
    for (int i = 1; i < argc; i++) {
        const char *a = argv[i], *v = i + 1 < argc ? argv[i + 1] : 0;
        if (!strcmp(a, "--disc") && v) port_disc_path = argv[++i];
        else if (!strcmp(a, "--data") && v) port_data_dir = argv[++i];
        else if (!strcmp(a, "--lang") && v) { port_lang = atoi(argv[++i]); if (port_lang < 0 || port_lang > 8) return usage(argv[0]); }
        else if (!strcmp(a, "--frames") && v) port_max_frames = atoi(argv[++i]);
        else if (!strcmp(a, "--headless")) headless = 1;
        else if (!strcmp(a, "--window")) headless = 0;
        else if (!strcmp(a, "--scale") && v) port_window_scale = atoi(argv[++i]);
        else if (!strcmp(a, "--fps60")) { extern int port_fps60; port_fps60 = 1; }
        else if (!strcmp(a, "--internal-res") && v) { extern int port_internal_res; port_internal_res = atoi(argv[++i]); if (port_internal_res < 1) port_internal_res = 1; if (port_internal_res > 4) port_internal_res = 4; }
        else if (!strcmp(a, "--renderer") && v) {
            extern int port_renderer;
            i++;
            if (!strcmp(argv[i], "gl") || !strcmp(argv[i], "opengl")) port_renderer = 1;
            else if (!strcmp(argv[i], "soft") || !strcmp(argv[i], "software")) port_renderer = 0;
            else return usage(argv[0]);
        }
        else if (!strcmp(a, "--realtime")) port_realtime = 1;
        else if (!strcmp(a, "--fullscreen")) port_window_fullscreen = 1;
        else if (!strcmp(a, "--aspect") && v) {
            extern int port_widescreen; const char *as = argv[++i];
            port_window_square = !strcmp(as, "square");
            if (!strcmp(as, "16:9")) port_widescreen = 53; else if (!strcmp(as, "21:9")) port_widescreen = 120; else if (!strcmp(as, "32:9")) port_widescreen = 267;
        }
        else if (!strcmp(a, "--widescreen")) {      /* experimental (docs/PORT.md "Widescreen"): 16:9, or 21:9 */
            extern int port_widescreen; port_widescreen = 53;
            if (v && (!strcmp(v, "16:9") || !strcmp(v, "21:9") || !strcmp(v, "32:9"))) { const char *ws = argv[++i]; if (!strcmp(ws, "21:9")) port_widescreen = 120; else if (!strcmp(ws, "32:9")) port_widescreen = 267; }
        }
        else if (!strcmp(a, "--keys") && v) port_window_keys(argv[++i]);
        else if (!strcmp(a, "--show-fps")) { port_show_fps = 1; if (v && (*v == '1' || *v == '2') && !v[1]) port_show_fps = atoi(argv[++i]); }
        else if (!strcmp(a, "--patch-tex") && v) { extern const char *port_patch_tex_path, *port_patch_tex_paths[]; extern int port_patch_tex_count; port_patch_tex_path = argv[++i]; if (port_patch_tex_count < 4) port_patch_tex_paths[port_patch_tex_count++] = port_patch_tex_path; }
        else if (!strcmp(a, "--title-bg") && v) { extern const char *port_title_bg_path; port_title_bg_path = argv[++i]; }   /* widescreen: wide title background (runtime/titlebg.c) */
        else if (!strcmp(a, "--test-null")) test_null = 1;
        else if (!strcmp(a, "--hz") && v) { port_hz = atoi(argv[++i]); if (port_hz != 50 && port_hz != 60) return usage(argv[0]); }
        else if (!strcmp(a, "--stall") && v) { port_stall_frame = atoi(argv[++i]); const char *s = strchr(argv[i], ':'); if (s) port_stall_ms = atoi(s + 1); }
        else if (!strcmp(a, "--pad-script") && v) port_pad_script(argv[++i]);
        else if (!strcmp(a, "--dump-frames") && v) port_dump_dir = argv[++i];
        else if (!strcmp(a, "--dump-every") && v) port_dump_every = atoi(argv[++i]);
        else if (!strcmp(a, "--dump-vram")) port_dump_vram = 1;
        else if ((!strcmp(a, "--dump-textures") || !strcmp(a, "--texture-pack")) && v) {   /* shims/texpack.c */
            extern const char *port_texdump_dir, *port_texpack_dir; extern int port_texpack_on; extern void port_texpack_stop(void);
            if (a[2] == 'd') port_texdump_dir = argv[++i]; else port_texpack_dir = argv[++i];
            if (!port_texpack_on) atexit(port_texpack_stop);
            port_texpack_on = 1;
        }
        else if (!strcmp(a, "--dump-draws") && v) { extern const char *port_drawdump_path; port_drawdump_path = argv[++i]; }
        else if (!strcmp(a, "--dump-ram") && v) port_dump_ram_add(argv[++i]);
        else if (!strcmp(a, "--dump-audio") && v) port_audio_path = argv[++i];
        else if (!strcmp(a, "--skip-movies")) port_skip_movies = 1;
        else if (!strcmp(a, "--skip-intro")) port_skip_movies = 2;
        else if (!strcmp(a, "--play-movies")) port_skip_movies = 0;     /* (the default now; kept for old scripts) */
        else if (!strcmp(a, "--poke") && v) port_poke_add(argv[++i]);
        else if (!strcmp(a, "--poke-every") && v) port_poke_add_every(argv[++i], 1);
        else if (!strcmp(a, "--menu") && v) port_menu_add(argv[++i]);
        else if (!strcmp(a, "--test-item") && v) { extern void port_test_item_add(const char *); port_test_item_add(argv[++i]); }
        else if (!strcmp(a, "--title") && v) port_title_add(argv[++i]);
        else if (!strcmp(a, "--movie") && v) { extern void port_movie_add(const char *); port_movie_add(argv[++i]); }
        else if (!strcmp(a, "--save-dir") && v) port_save_dir = argv[++i];
        else if (!strcmp(a, "--trace-cd")) port_trace_cd = 1;
        else if (!strcmp(a, "-v")) port_verbose++;
        else return usage(argv[0]);
    }
    if (headless) port_window = 0;
    { extern int port_widescreen, port_renderer, port_ws_cull;
      port_ws_cull = port_widescreen;
      (void)port_renderer; }
    setvbuf(stderr, NULL, _IONBF, 0);     /* the launcher reads a pipe: every line must leave the process at once */
    { extern const char port_build_id[], port_build_date[]; port_printf("[port2] build %s %s\n", port_build_id, port_build_date); }
    port_printf("[port2] native pointers, %d-bit: data image %p..%p, scratch %p, layout %s\n", (int)(sizeof(void *) * 8),
                (void *)port_main0_image, (void *)port_x000_bss, (void *)port_scratch,
                port_check_layout() ? "ok (below 16 MB)" : "NOT below 16 MB: OT links would truncate");
    if (!port_check_layout()) return 2;
    { extern void port_lowmem_install(void); port_lowmem_install(); }
    if (test_null) {     /* --test-null: the fault handlers (null-page emulation, division by zero) on this host */
        volatile unsigned char *p = (volatile unsigned char *)0x1234; volatile int zero = 0, q;
        *p = 0x5A; q = 100 / zero;
        port_printf("[port2] test-null: wrote 0x5A through 0x1234, port_ram[0x1234] = 0x%02X; 100/0 = %d (R3000: -1)\n",
                    ((unsigned char *)port_ram)[0x1234], q);
        /* the same from a coroutine stack (the game's tasks fault there, OpenTh/ChangeTh): read, write, division */
        extern int port_test_null_coroutine(void);
        int co = port_test_null_coroutine();
        { char low[256]; port_low_second(low, sizeof low); port_printf("[port2] test-null: the fps line's field would read \"%s\"\n", low + 1); }
        port_low_report();
        return ((unsigned char *)port_ram)[0x1234] == 0x5A && q == -1 && co == 0 ? 0 : 3;
    }
    if (port_window) { port_window_start(); port_realtime = 1; }
    port_gpu_start();
    open_game_data(argv[0]);
    /* what the boot executable (SCES_013.31) leaves in the scratchpad before LoadExec (notes/cdfiles.md) */
    SPAD16(0x3D8) = 1;
    SPAD16(0x3DA) = 7;
    port_lang_init();          /* --lang: MAINn's file table and data over MAIN0's (runtime/lang.c) */
    port_watchdog_start();     /* after SDL_Init: a slow driver start must not count as a hang */
    game_main();
    return 0;
}
