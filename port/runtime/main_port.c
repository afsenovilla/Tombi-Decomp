/* Port entry: sets up the PS1 address space from the user's MAIN0.EXE, then runs the game's main().
 *
 * main() (0x800163F4, 1396 bytes) is the one game function the matching decomp keeps as Psy-Q-classified asm, so
 * the port carries this C translation of it (from build/full/asm/main0/nonmatchings/main0/main.s).  The only
 * changes: the vblank busy-wait calls port_vblank() (there is no interrupt to advance the counter), and the
 * frame limit / exit is handled by the runtime. */
#include "port_runtime.h"

typedef unsigned char u8; typedef unsigned short u16; typedef short s16; typedef unsigned int u32;

#define SPAD8(a) (*(volatile u8 *)(0x1F800000u + (a)))
#define SPAD16(a) (*(volatile u16 *)(0x1F800000u + (a)))
#define SPAD32(a) (*(volatile u32 *)(0x1F800000u + (a)))

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
extern void port_setup_memory(void);
extern int port_realtime;

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
    port_printf("[port] game main: init done, entering the main loop\n");

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

static int streq(const char *a, const char *b) { while (*a && *a == *b) { a++; b++; } return *a == *b; }
static int atoi_(const char *s) { int v = 0; while (*s >= '0' && *s <= '9') v = v * 10 + *s++ - '0'; return v; }

static void load_exe(const char *path) {
    int fd = port_open(path);
    if (fd < 0) port_fatal("cannot open %s (your own copy of EXE/MAIN0.EXE; see docs/PORT.md)", path);
    u8 hdr[0x800];
    if (port_read(fd, hdr, 0x800) != 0x800 || hdr[0] != 'P' || hdr[1] != 'S') port_fatal("%s is not a PS-X EXE", path);
    u32 pc = *(u32 *)(hdr + 0x10), t_addr = *(u32 *)(hdr + 0x18), t_size = *(u32 *)(hdr + 0x1C);
    if (t_addr < PSX_RAM_BASE || t_addr + t_size > PSX_RAM_BASE + PSX_RAM_SIZE) port_fatal("bad EXE layout");
    if (port_read(fd, (void *)t_addr, t_size) != (int)t_size) port_fatal("short read on %s", path);
    port_close(fd);
    port_printf("[port] %s: %u bytes at %08x (entry %08x)\n", path, t_size, t_addr, pc);
}

int port_main(int argc, char **argv) {
    const char *exe = "game/MAIN0.EXE", *disc = "game/disc/Tombi.bin";
    for (int i = 1; i < argc; i++) {
        if (streq(argv[i], "--exe") && i + 1 < argc) exe = argv[++i];
        else if (streq(argv[i], "--disc") && i + 1 < argc) disc = argv[++i];
        else if (streq(argv[i], "--frames") && i + 1 < argc) port_max_frames = atoi_(argv[++i]);
        else if (streq(argv[i], "--realtime")) port_realtime = 1;
        else if (streq(argv[i], "--autostart") && i + 1 < argc) { extern int port_autostart; port_autostart = atoi_(argv[++i]); }
        else if (streq(argv[i], "--pad-script") && i + 1 < argc) { extern void port_pad_script(const char *); port_pad_script(argv[++i]); }
        else if (streq(argv[i], "--dump-frames") && i + 1 < argc) port_dump_dir = argv[++i];
        else if (streq(argv[i], "--dump-every") && i + 1 < argc) port_dump_every = atoi_(argv[++i]);
        else if (streq(argv[i], "--dump-vram")) port_dump_vram = 1;
        else if (streq(argv[i], "--play-movies")) { extern int port_play_movies; port_play_movies = 1; }
        else if (streq(argv[i], "--no-interp")) port_interp = 0;
        else if (streq(argv[i], "--trace-interp")) { extern int port_mips_trace; port_mips_trace = 1; }
        else if (streq(argv[i], "-v")) port_verbose++;
        else {
            port_printf("usage: %s [--exe MAIN0.EXE] [--disc Tombi.bin|.iso] [--frames N] [--realtime]\n"
                        "       [--dump-frames DIR [--dump-every N] [--dump-vram]] [--play-movies] [-v]\n"
                        "       [--pad-script FRAME:BUTTONS[:LEN],...]   e.g. 1000:start,1100:cross:6\n", argv[0]);
            return 2;
        }
    }
    port_setup_memory();
    load_exe(exe);
    if (port_cd_open(disc) < 0) port_printf("[port] warning: no disc image at %s: CD reads will fail\n", disc);
    /* what the boot executable (SCES_013.31) leaves in the scratchpad before LoadExec (notes/cdfiles.md) */
    SPAD16(0x3D8) = 1;
    SPAD16(0x3DA) = 7;
    game_main();
    return 0;
}
