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

void port_printf(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt); vfprintf(stderr, fmt, ap); va_end(ap);
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
}
void port_quit(int code) {
    port_window_stop();
    exit(code);
}

unsigned long long port_time_us(void) {
    struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
    return (unsigned long long)ts.tv_sec * 1000000ull + ts.tv_nsec / 1000;
}
void port_sleep_us(unsigned long long us) {
    struct timespec ts = { us / 1000000ull, (us % 1000000ull) * 1000 };
    while (nanosleep(&ts, &ts) && errno == EINTR) ;
}
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
const char *port_disc_path = "game/disc/Tombi.bin";
static int usage(const char *argv0) {
    port_printf("usage: %s [--disc Tombi.bin|.cue|.iso] [--frames N] [--headless] [--window [--scale N]] [--realtime] [--hz 50|60]\n"
                "       [--dump-frames DIR [--dump-every N] [--dump-vram]] [--pad-script FRAME:BUTTONS[:LEN[xCOUNT/PERIOD]],...]\n"
                "       [--poke FRAME:ADDR:VALUE[:SIZE[xCOUNT]]] [--poke-every INTERVAL:ADDR:VALUE[:SIZE]] [--menu FRAME:N]\n"
                "       [--dump-audio PATH] [--play-movies] [--trace-cd] [--save-dir DIR] [-v]\n"
                "  the default is a window (SDL2) paced at 50 Hz; --headless runs as fast as possible without one\n", argv0);
    return 2;
}
int main(int argc, char **argv) {
    extern void port_pad_script(const char *), port_poke_add(const char *), port_poke_add_every(const char *, int),
        port_menu_add(const char *), port_dump_ram_add(const char *);
    extern const char *port_audio_path, *port_save_dir; extern int port_play_movies, port_show_fps, port_stall_frame, port_stall_ms;
    int headless = 0;
    port_window = 1;
    for (int i = 1; i < argc; i++) {
        const char *a = argv[i], *v = i + 1 < argc ? argv[i + 1] : 0;
        if (!strcmp(a, "--disc") && v) port_disc_path = argv[++i];
        else if (!strcmp(a, "--frames") && v) port_max_frames = atoi(argv[++i]);
        else if (!strcmp(a, "--headless")) headless = 1;
        else if (!strcmp(a, "--window")) headless = 0;
        else if (!strcmp(a, "--scale") && v) port_window_scale = atoi(argv[++i]);
        else if (!strcmp(a, "--realtime")) port_realtime = 1;
        else if (!strcmp(a, "--show-fps")) port_show_fps = 1;
        else if (!strcmp(a, "--hz") && v) { port_hz = atoi(argv[++i]); if (port_hz != 50 && port_hz != 60) return usage(argv[0]); }
        else if (!strcmp(a, "--stall") && v) { port_stall_frame = atoi(argv[++i]); const char *s = strchr(argv[i], ':'); if (s) port_stall_ms = atoi(s + 1); }
        else if (!strcmp(a, "--pad-script") && v) port_pad_script(argv[++i]);
        else if (!strcmp(a, "--dump-frames") && v) port_dump_dir = argv[++i];
        else if (!strcmp(a, "--dump-every") && v) port_dump_every = atoi(argv[++i]);
        else if (!strcmp(a, "--dump-vram")) port_dump_vram = 1;
        else if (!strcmp(a, "--dump-ram") && v) port_dump_ram_add(argv[++i]);
        else if (!strcmp(a, "--dump-audio") && v) port_audio_path = argv[++i];
        else if (!strcmp(a, "--play-movies")) port_play_movies = 1;
        else if (!strcmp(a, "--poke") && v) port_poke_add(argv[++i]);
        else if (!strcmp(a, "--poke-every") && v) port_poke_add_every(argv[++i], 1);
        else if (!strcmp(a, "--menu") && v) port_menu_add(argv[++i]);
        else if (!strcmp(a, "--save-dir") && v) port_save_dir = argv[++i];
        else if (!strcmp(a, "--trace-cd")) port_trace_cd = 1;
        else if (!strcmp(a, "-v")) port_verbose++;
        else return usage(argv[0]);
    }
    if (headless) port_window = 0;
    port_printf("[port2] native pointers, %d-bit: data image %p..%p, scratch %p, layout %s\n", (int)(sizeof(void *) * 8),
                (void *)port_main0_image, (void *)port_x000_bss, (void *)port_scratch,
                port_check_layout() ? "ok (below 16 MB)" : "NOT below 16 MB: OT links would truncate");
    if (!port_check_layout()) return 2;
    { extern void port_lowmem_install(void); port_lowmem_install(); }
    if (port_window) { port_window_start(); port_realtime = 1; }
    if (port_cd_open(port_disc_path) < 0) port_printf("[port2] warning: no disc image at %s: CD reads will fail\n", port_disc_path);
    /* what the boot executable (SCES_013.31) leaves in the scratchpad before LoadExec (notes/cdfiles.md) */
    SPAD16(0x3D8) = 1;
    SPAD16(0x3DA) = 7;
    game_main();
    return 0;
}
