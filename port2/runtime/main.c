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
#include <sys/stat.h>
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
    port_printf("usage: %s [--data DIR | --disc Tombi.bin|.cue|.iso] [--lang N] [--cut-content] [--frames N] [--headless] [--window [--scale N] [--aspect 4:3|square|16:9|21:9|32:9] [--widescreen [16:9|21:9|32:9]] [--fullscreen] [--keys up=Up,cross=X,...]]\n"
                "       [--renderer gl|soft] [--internal-res 1..4] [--fps60] [--realtime] [--hz 50|60] [--show-fps [1|2]] [--patch-tex FILE]... [--title-bg FILE] [--early-title PREPRE8_DIR]\n"
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
/* --civ-test (dev): stand-in load list for the cut AREA11 section 0, a copy of SYS/LD0AR08.BIN list 1 (AREA08 s0),
   with its sprite page replaced by the surviving Lumberjack page (file 155 = SYSTEM/A06100, as SYS list 61 loads it) and
   its CLUT block by AREA11/CLUT02 (file 839):
   {file id, slot, kind, addr, size, w, h, type}; file ids are global (FILE_LINKS), so they load from any area.
   AREA08's villager page B000 (file 699, at x 64 in AREA08) is also loaded at VRAM x 512 for the stand-ins (w1e + 7).
   spawnSectionObjectsAlt skips the area-wide APD pass for it (fixes.txt): AREA11 would read slot 5, never loaded here */
int port_civ_test;
static const struct { short id; unsigned char slot, kind; unsigned addr, size; short w, h; unsigned type; } port_civ_recs[] = {
    {155, 128, 0x10, 0, 0x40, 448, 256, 16}, {700, 128, 0x10, 0, 0x3c0, 64, 256, 1}, {839, 128, 0x10, 0, 0x1e00080, 256, 32, 1},
    {702, 128, 0x8e, 0, 0x4008, 0, 0, 4}, {0, 255, 0x8f, 0, 0x3ba20, 0, 0, 0xffffffffu}, {0, 255, 0x62, 0, 0xca3c, 0, 0, 0xffffffffu},
    {703, 255, 0x3a, 0, 0x11dc, 0, 0, 3}, {0, 255, 0x3b, 0, 0x2654, 0, 0, 0xffffffffu}, {0, 255, 0x3c, 0, 0x260, 0, 0, 0xffffffffu},
    {0, 255, 0x3d, 0, 0x190, 0, 0, 0xffffffffu}, {0, 255, 0x3e, 0, 0xc7c, 0, 0, 0xffffffffu}, {0, 255, 0x80, 0, 0x80, 0, 0, 0xffffffffu},
    {0, 255, 0x84, 0, 0x20, 0, 0, 0xffffffffu},
    {699, 128, 0x10, 0, 0x200, 192, 256, 1}, {704, 255, 0xf0, 0, 0x218dc, 0, 0, 2}, {-1, 0, 0, 0, 0, 0, 0, 0}};
static unsigned char port_civ_buf[sizeof port_civ_recs];
/* --civ-test spawn: AREA08 s0's row, optionally moved with --civ-spawn X,Y (world) to stand next to a resident */
/* SPR table for AREA11 subtype 0 (frames 0..29 of its scripts: idle 0-2, walk 3-20, talk 21-29), built at runtime from
   boxes on the surviving A06100 4 bpp page (page 0 relative, w1e 1 = VRAM x 0x40), CLUT AREA11/CLUT02 row 4 (VRAM
   (192, 480)): port_civ_fig(11, 0), frame order from the worker's GIFs (port_civ_worker below). Boxes from
   tools/civ_sheets.py comps(); no pixels. */
int port_civ_fig(int area, int sub);
int port_civ_spr(void) { return port_civ_fig(11, 0); }
/* Real sprites of AREA16 subtypes 0..2 and AREA17 subtype 0 (docs/CIVILIZATION.md): one standing figure of the
   surviving page (w1e 1 = VRAM x 64, boxes from tools/civ_sheets.py comps()) with its real CLUT row (VRAM
   (128 + 16 * (r % 16), 480 + r / 16)): AREA16/CLUT07 rows 21 / 37 / 53, AREA17/CLUT03 row 1. Their tables are lost;
   a frame index of the anim scripts with no entry in port_civ_anim points to this part (the figure stands still). */
static const struct { unsigned char area, sub, u, v, w, h, row; } port_civ_figs[] = {
    {16, 0, 67, 48, 36, 45, 21}, {16, 1, 145, 148, 24, 35, 37}, {16, 2, 75, 203, 50, 52, 53}, {17, 0, 25, 3, 22, 45, 1}};
static unsigned char port_civ_fig_buf[5][128 * 4 + 32 * 16];
/* Rebuilt frame tables (tools/civ_anim.py): frame ids first..first+n-1 of the I0NFO2 anim scripts (idle 3, walk 3x6,
   talk 3x2 or 3x3; facing order back, side, front) -> part box on the page, flip = mirrored (texels at u - 1). The order
   comes from the community's GIFs of these residents, matched pixel-exact against the page; the walk-back/front
   frames 4..6 are frames 1..3 mirrored, as in the worker's GIFs. */
struct port_civ_part { unsigned char u, v, w, h, flip; };
static const struct port_civ_part port_civ_worker[30] = {   /* AREA11 sub 0 (Lumberjack Factory Worker), frames 0..29 */
    {1,0,22,43,0}, {27,0,17,45,0}, {49,0,22,44,0}, {74,0,19,43,0}, {97,0,21,43,0}, {121,0,22,43,0}, {74,0,19,43,1},
    {97,0,21,43,1}, {121,0,22,43,1}, {151,0,24,43,0}, {179,0,17,43,0}, {203,0,18,43,0}, {231,0,24,43,0}, {3,48,17,44,0},
    {27,48,18,44,0}, {51,48,19,45,0}, {73,48,22,45,0}, {97,48,22,43,0}, {51,48,19,45,1}, {73,48,22,45,1},
    {97,48,22,43,1}, {124,48,23,43,0}, {153,48,21,43,0}, {177,49,22,42,0}, {201,48,20,45,0}, {225,48,24,45,0},
    {1,96,26,45,0}, {36,96,24,44,0}, {66,96,21,44,0}, {89,96,22,45,0}};
static const struct port_civ_part port_civ_woman[27] = {    /* AREA16 sub 0 (Female Stonetown Resident), frames 30..56 */
    {225,48,29,45,0}, {234,96,18,46,0}, {49,144,29,46,0}, {2,0,37,45,0}, {42,0,35,45,0}, {81,0,37,43,0}, {2,0,37,45,1},
    {42,0,35,45,1}, {81,0,37,43,1}, {122,0,23,45,0}, {153,0,24,46,0}, {186,0,26,45,0}, {217,0,24,45,0}, {2,48,23,44,0},
    {33,48,28,43,0}, {67,48,36,45,0}, {107,48,36,44,0}, {147,48,32,44,0}, {67,48,36,45,1}, {107,48,36,44,1},
    {147,48,32,44,1}, {186,49,37,44,0}, {2,96,37,45,0}, {65,96,20,47,0}, {234,96,18,46,0}, {91,97,39,45,0},
    {138,96,41,46,0}};
/* AREA16 sub 1 (the boy), frames 60..86: walk front = figures 1, 2, 2, 1, 2 mirrored, 2 mirrored (the mirrored step rebuilds the missing
   other-leg frame), walk side = 3-6 (frame order given by the project
   owner from the page); no back, idle or talk drawings on the page, so idle/talk reuse 1 (front) and 3 (side) */
static const struct port_civ_part port_civ_boy[27] = {
    {113,148,26,35,0}, {176,146,20,37,0}, {113,148,26,35,0}, {113,148,26,35,0}, {145,148,24,35,0}, {145,148,24,35,0}, {113,148,26,35,0}, {145,148,24,35,1}, {145,148,24,35,1}, {176,146,20,37,0}, {226,147,21,37,0}, {209,185,21,36,0}, {232,186,23,36,0}, {176,146,20,37,0}, {226,147,21,37,0}, {113,148,26,35,0}, {145,148,24,35,0}, {145,148,24,35,0}, {113,148,26,35,0}, {145,148,24,35,1}, {145,148,24,35,1}, {113,148,26,35,0}, {113,148,26,35,0}, {176,146,20,37,0}, {176,146,20,37,0}, {113,148,26,35,0}, {113,148,26,35,0}
};
/* AREA17 sub 0 (Iron Town resident), frames 0..29: walk down 1-6, up 7-12, side 13-18, idle front 20, wave front
   21-23, side 24-26, back 27-29 (figure numbers of the A08000 sheet, order given by the project owner) */
static const struct port_civ_part port_civ_iron[30] = {
    {145,2,21,46,0}, {49,51,26,45,0}, {33,97,22,47,0}, {145,2,21,46,0}, {169,3,21,44,0}, {193,4,22,44,0}, {217,2,21,46,0}, {1,51,20,44,0}, {25,52,21,44,0}, {49,51,26,45,0}, {81,51,24,45,0}, {114,51,24,45,0}, {145,51,25,45,0}, {177,51,24,44,0}, {210,51,24,45,0}, {1,4,20,43,0}, {25,3,22,45,0}, {49,3,22,45,0}, {73,4,20,43,0}, {97,3,22,45,0}, {121,3,22,45,0}, {5,148,26,44,0}, {36,149,27,43,0}, {71,150,24,42,0}, {154,98,24,46,0}, {185,99,22,45,0}, {211,98,24,46,0}, {59,99,28,45,0}, {92,99,27,45,0}, {126,99,25,45,0}};
static const struct { unsigned char area, sub, first, n, row; const struct port_civ_part *t; } port_civ_anim[] = {
    {11, 0, 0, 30, 4, port_civ_worker}, {16, 0, 30, 27, 21, port_civ_woman}, {16, 1, 60, 27, 37, port_civ_boy},
    {17, 0, 0, 30, 1, port_civ_iron}};
static void port_civ_put_part(unsigned char *e, const struct port_civ_part *q, unsigned clut) {
    int u0 = q->flip ? q->u + q->w - 1 : q->u, u1 = q->flip ? q->u - 1 : q->u + q->w;
    e[0] = u0; e[1] = q->v; e[2] = clut & 0xFF; e[3] = clut >> 8;
    e[4] = u1; e[5] = q->v; e[6] = 0; e[7] = 0;
    e[8] = u0; e[9] = q->v + q->h; e[10] = q->w; e[11] = q->h;
    e[12] = u1; e[13] = q->v + q->h; e[14] = (unsigned char)(-(q->w / 2)); e[15] = (unsigned char)(-q->h);
}
/* stand-in villager CLUT (b0d 1, w08): one real resident row of the town's CLUT block, since the villager's own rows
   (AREA08/CLUT01 1, 17, 49, 97, 113, 129) are not loaded: AREA11 row 36 (the old man), AREA16 row 54 and AREA17
   row 49 (the red-shirt resident of the FMV, docs/CIVILIZATION.md) */
int port_civ_standin_clut(int area) {
    int r = area == 11 ? 36 : area == 16 ? 54 : 49;
    return ((480 + r / 16) << 6) | ((128 + 16 * (r % 16)) >> 4);
}
int port_civ_fig(int area, int sub) {
    int i, f, a, k = -1;
    const struct port_civ_part *t = 0;
    struct port_civ_part q;
    unsigned clut, off = 128 * 4;
    unsigned char *p;
    for (i = 0; i < 4; i++) if (port_civ_figs[i].area == area && port_civ_figs[i].sub == sub) break;
    for (a = 0; a < 4; a++) if (port_civ_anim[a].area == area && port_civ_anim[a].sub == sub) t = port_civ_anim[a].t, k = a;
    if (i == 4 && !t) return 0;
    p = port_civ_fig_buf[i == 4 ? 4 : i];
    if (i < 4) { q.u = port_civ_figs[i].u; q.v = port_civ_figs[i].v; q.w = port_civ_figs[i].w; q.h = port_civ_figs[i].h; }
    else q = t[0];
    q.flip = 0;
    {
        int r = i < 4 ? port_civ_figs[i].row : port_civ_anim[k].row;
        clut = ((480u + r / 16) << 6) | ((128u + 16 * (r % 16)) >> 4);
    }
    port_civ_put_part(p + off, &q, clut);
    for (f = 0; f < 128; f++) { p[4 * f] = 1; p[4 * f + 1] = 0; p[4 * f + 2] = off & 0xFF; p[4 * f + 3] = off >> 8; }
    if (t)
        for (f = 0; f < port_civ_anim[k].n; f++) {
            unsigned o = off + 16 * (f + 1), id = port_civ_anim[k].first + f;
            port_civ_put_part(p + o, &t[f], clut);
            p[4 * id + 2] = o & 0xFF; p[4 * id + 3] = o >> 8;
        }
    return (int)(long)p;
}
/* --civ-wfm FILE: replaces AREA11's dialogue bank (WFM3_DATA[0] = scratchpad 0x1F800398) before a set-0D resident talks */
static const char *port_civ_wfm_path;
int port_civ_line = -1;   /* --civ-line N: bank-4 line shown by every set-0D resident (dev) */
static unsigned char *port_civ_wfm_buf;
void port_civ_wfm_swap(void) {   /* --civ-wfm FILE (AREA11's bank) or DIR (DIR/civ_MFNTnn0S.WFM of the current town) */
    extern void *port_ram_ptr(unsigned);
    static int warea = -1;
    int area = *(unsigned short *)port_ram_ptr(0x8009C960u);
    char path[1024]; struct stat st;
    if (!port_civ_test || !port_civ_wfm_path) return;
    if (!port_civ_wfm_buf || warea != area) {
        FILE *f; long n;
        if (!stat(port_civ_wfm_path, &st) && S_ISDIR(st.st_mode)) snprintf(path, sizeof path, "%s/civ_MFNT%02d0S.WFM", port_civ_wfm_path, area);
        else if (area == 11) snprintf(path, sizeof path, "%s", port_civ_wfm_path);
        else return;
        f = fopen(path, "rb");
        if (!f) { port_printf("[port] --civ-wfm: cannot open %s\n", path); return; }
        fseek(f, 0, SEEK_END); n = ftell(f); fseek(f, 0, SEEK_SET);
        free(port_civ_wfm_buf); port_civ_wfm_buf = malloc(n + 16);
        if (fread(port_civ_wfm_buf, 1, n, f) != (size_t)n) { fclose(f); free(port_civ_wfm_buf); port_civ_wfm_buf = 0; return; }
        fclose(f); warea = area;
        if (port_verbose) port_printf("[port] --civ-wfm: %s (%ld bytes)\n", path, n);
    }
    *(unsigned char **)port_ram_ptr(0x1F800398u) = port_civ_wfm_buf;
}
static int port_civ_cur_area(void) { extern void *port_ram_ptr(unsigned); return *(unsigned short *)port_ram_ptr(0x8009C960u); }
/* which cut town is being tested: AREA11, AREA16 or AREA17 s0, all with the AREA08 s0 stand-in list (--civ-town N
   opens the debug area select on it) */
int port_civ_on(int area, int sec) {   /* area: GAME.selectedArea (the load list index may add AREA_VARIANT: re-read it) */
    extern void *port_ram_ptr(unsigned);
    area = *(unsigned short *)port_ram_ptr(0x8009C960u);
    return port_civ_test && sec == 0 && (area == 11 || area == 16 || area == 17);
}
/* --civ-maps DIR: DIR/AREAnn/map8E.bin and map8F.tim (tools/civ_rebuild.py) replace APD_DATA[14] / [15] */
static const char *port_civ_maps_dir;
static int port_civ_map_w, port_civ_map_h;   /* rebuilt picture size in px (0: the stand-in AREA08 one) */
static void *port_civ_slurp(const char *path) {
    FILE *f = fopen(path, "rb"); long n; void *b;
    if (!f) return 0;
    fseek(f, 0, SEEK_END); n = ftell(f); fseek(f, 0, SEEK_SET);
    b = malloc(n + 16);
    if (fread(b, 1, n, f) != (size_t)n) { fclose(f); free(b); return 0; }
    fclose(f); return b;
}
/* --civ-collision: the debug pictures (map8F_cells.tim / map8F_in_cells.tim: the cell map tinted over the picture,
   red solid, orange diagonal, cyan trigger) replace the pictures. The room view (map8F_in.tim, the house interiors on
   black) is shown while Tomba is inside a house (port_civ_room_pixels, topdownRefreshMap rule in fixes.txt). */
int port_civ_collision;
static void *port_civ_tim_out, *port_civ_tim_in;
static const char *port_civ_tim_name(int in) {
    return port_civ_collision ? (in ? "map8F_in_cells.tim" : "map8F_cells.tim") : (in ? "map8F_in.tim" : "map8F.tim");
}
unsigned long *port_civ_room_pixels(int room) {
    void *t = room ? port_civ_tim_in : port_civ_tim_out;
    if (!port_civ_test || !t) return 0;
    return (unsigned long *)((char *)t + 0x220);
}
void port_civ_maps(void) {
    extern void *port_ram_ptr(unsigned);
    char path[1024]; void *e, *t; int area = *(unsigned short *)port_ram_ptr(0x8009C960u);
    static void *ce, *ct; static int carea = -1;
    if (!port_civ_maps_dir || !port_civ_on(area, *(unsigned short *)port_ram_ptr(0x8009C962u))) return;
    if (carea != area) {
        snprintf(path, sizeof path, "%s/AREA%02d/map8E.bin", port_civ_maps_dir, area); e = port_civ_slurp(path);
        snprintf(path, sizeof path, "%s/AREA%02d/%s", port_civ_maps_dir, area, port_civ_tim_name(0)); t = port_civ_slurp(path);
        if (!e || !t) { port_printf("[port] --civ-maps: no rebuilt map for AREA%02d in %s\n", area, port_civ_maps_dir); return; }
        ce = e; ct = t; carea = area;
        snprintf(path, sizeof path, "%s/AREA%02d/%s", port_civ_maps_dir, area, port_civ_tim_name(1));
        port_civ_tim_out = t; port_civ_tim_in = port_civ_slurp(path);   /* none: rooms keep the engine's hidden map */
        port_civ_map_w = ((unsigned short *)ce)[0] * 8; port_civ_map_h = ((unsigned short *)ce)[1] * 8;
        if (port_verbose) port_printf("[port] --civ-maps: AREA%02d rebuilt map %dx%d px\n", area, port_civ_map_w, port_civ_map_h);
    }
    *(void **)port_ram_ptr(0x1F800390u) = ce;
    *(void **)port_ram_ptr(0x1F800394u) = ct;
}
/* camera limits {xmin, xmax, ymin, ymax} and start for the rebuilt picture (AREA08's rows follow xmax = W - 288,
   ymax = H - 112); spawn at the bottom exit of AREA17 (the way in from Civilization Town) */
static short port_civ_lim[4], port_civ_campos[8];
static int port_civ_spawn_set, port_civ_spawn_x, port_civ_spawn_y;
static void port_civ_default_spawn(int *x, int *y) {
    if (port_civ_spawn_set) { *x = port_civ_spawn_x; *y = port_civ_spawn_y; return; }
    if (port_civ_maps_dir) switch (port_civ_cur_area()) {   /* just inside the exit from Civilization Town (AREA18) */
    case 17: *x = 223; *y = 540; return;   /* above exit 0 (y > 0x23A) */
    case 11: *x = 530; *y = 410; return;   /* west of exit 0 (x > 555, y 361..460) */
    case 16: *x = -95; *y = 524; return;   /* east of exit 0 (x < -116, y 520..529) */
    }
    *x = 60; *y = 468;                                                                      /* AREA08 s0 spawn 0 */
}
short *port_civ_limits(short *row) {
    memcpy(port_civ_lim, row, sizeof port_civ_lim);
    port_civ_maps();   /* size of the rebuilt picture (loads the files once) */
    if (port_civ_map_w) { port_civ_lim[1] = port_civ_map_w - 288; port_civ_lim[3] = port_civ_map_h - 112; }
    return port_civ_lim;
}
short *port_civ_cam(short *row) {
    int x, y;
    memcpy(port_civ_campos, row, sizeof port_civ_campos);
    port_civ_default_spawn(&x, &y); port_civ_campos[0] = x; port_civ_campos[1] = y;
    return port_civ_campos;
}
static short port_civ_row_buf[4];
short *port_civ_row(short *row) {
    int x, y;
    memcpy(port_civ_row_buf, row, sizeof port_civ_row_buf);
    port_civ_default_spawn(&x, &y); port_civ_row_buf[0] = x; port_civ_row_buf[1] = y;
    return port_civ_row_buf;
}
int port_civ_list(void) {
    memcpy(port_civ_buf, port_civ_recs, sizeof port_civ_recs);
    /* each town's surviving page (file id, VRAM x 64) and its area CLUT block: Stonetown SYSTEM/A07600 (320 words) +
       AREA16/CLUT07, Iron Town SYSTEM/A08000 + AREA17/CLUT03 (AREA11: A06100 + AREA11/CLUT02, the table's default) */
    if (port_civ_cur_area() == 16) {
        port_civ_buf[0] = 182 & 0xFF; port_civ_buf[1] = 182 >> 8; port_civ_buf[12] = 320 & 0xFF; port_civ_buf[13] = 320 >> 8;
        port_civ_buf[40] = 1027 & 0xFF; port_civ_buf[41] = 1027 >> 8;
    } else if (port_civ_cur_area() == 17) {
        port_civ_buf[0] = 192 & 0xFF; port_civ_buf[1] = 192 >> 8;
        port_civ_buf[40] = 1046 & 0xFF; port_civ_buf[41] = 1046 >> 8;
    }
    if (port_verbose) port_printf("[port] --civ-test: AREA%02d s0 loads AREA08 s0's map, sprites and I0NFO2\n", port_civ_cur_area());
    return (int)(long)port_civ_buf;
}
int port_cut_content;     /* --cut-content: opt-in cut text recovered from the Japanese release (port/fixes.txt) */
int port_cc_boatman = 3;  /* AREA00 boatman: extra lines (7,3..5) still to show */
/* --cut-content berries: objects spawnItemFromEntry made (table drops) that initItemObject has not set up yet */
static void *port_cc_drops[16];
static unsigned port_cc_drop_n, port_cc_rng = 0x7a3b1c9u;
void port_cc_drop_mark(void *o) { if (port_cut_content) port_cc_drops[port_cc_drop_n++ & 15] = o; }
int port_cc_drop_take(void *o) {
    int i;
    for (i = 0; i < 16; i++) if (port_cc_drops[i] == o) { port_cc_drops[i] = 0; return port_cut_content; }
    return 0;
}
/* fruit drop colour: 1/32 gold (b0c 3), 4/32 = 1/8 silver (b0c 2), else plain (0); own LCG, game RNG untouched */
int port_cc_berry_roll(void) {
    unsigned r;
    port_cc_rng = port_cc_rng * 1103515245u + 12345u;
    r = (port_cc_rng >> 16) & 31;
    if (r == 0) { if (port_verbose) port_printf("[port] --cut-content: golden berry\n"); return 3; }
    if (r <= 4) { if (port_verbose) port_printf("[port] --cut-content: silver berry\n"); return 2; }
    return 0;
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
        else if (!strcmp(a, "--cut-content")) port_cut_content = 1;
        else if (!strcmp(a, "--civ-test")) port_civ_test = 1;
        else if (!strcmp(a, "--civ-route")) {
            /* opt-in cut route (port only): the Civilization Town hub (AREA18 s1) exits 0/1/2 lead into the cut
               section 0 of AREA11 / AREA17 / AREA16 instead of 11:1 / 17:1 / 16:4, and Stonetown's top exit
               (AREA16 s0 exit 2, to the cut 16:1) continues to 16:4; the other town exits already lead on to 11:1
               and 17:1 and back to 18:1. Exit records D_80079A48 {x, y, kind, area, section, spawn}, rewritten every
               second (MAIN0 data). Needs --civ-maps. */
            port_civ_test = 1;
            port_poke_add_every("50:800799E6:0", 1); port_poke_add_every("50:800799EE:0", 1);
            port_poke_add_every("50:800799F6:0", 1); port_poke_add_every("50:80079906:4", 1);
        }
        else if (!strcmp(a, "--civ-town") && v) {   /* --civ-test + the debug area select opened on AREA N s0 */
            char b[48]; int n = atoi(argv[++i]);
            if (n != 11 && n != 16 && n != 17) return usage(argv[0]);
            port_civ_test = 1;
            port_poke_add("900:1F8001B4:1");
            snprintf(b, sizeof b, "1250:8009C960:%X:2", n); port_poke_add(b);
            port_poke_add("1250:8009C962:0:2");
            port_pad_script("1000:start,1300:circle:3");
        }
        else if (!strcmp(a, "--civ-wfm") && v) port_civ_wfm_path = argv[++i];
        else if (!strcmp(a, "--civ-line") && v) port_civ_line = atoi(argv[++i]);
        else if (!strcmp(a, "--civ-maps") && v) port_civ_maps_dir = argv[++i];
        else if (!strcmp(a, "--civ-collision")) port_civ_collision = 1;
        else if (!strcmp(a, "--civ-spawn") && v) { port_civ_spawn_set = sscanf(argv[++i], "%d,%d", &port_civ_spawn_x, &port_civ_spawn_y) == 2; }
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
        else if (!strcmp(a, "--early-title") && v) { extern const char *port_early_title_dir; port_early_title_dir = argv[++i]; }   /* --cut-content: PrePre Vol. 8 title landscape (runtime/titlebg.c) */
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
