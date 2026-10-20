/* Psy-Q library replacements for the port prototype (headless).
 * Grouped like the Psy-Q libraries: libapi (BIOS: events, threads, critical sections), libetc (VSync,
 * callbacks), libgpu (VRAM transfers, ordering tables, environments), libgte (matrix helpers on the software
 * GTE), libcd (sector reads from the user's disc image), libspu/libsnd/libpress (silent stubs; the memory card is card.c).
 * Every function here overrides the weak call thunk that port/tools/gen_port.py generates for its name. */
#include "port_runtime.h"

typedef unsigned char u8; typedef unsigned short u16; typedef short s16; typedef int s32; typedef unsigned int u32;
typedef struct { s16 x, y, w, h; } RECT;
void *memcpy(void *d, const void *s, unsigned int n);
typedef struct { s16 m[3][3]; s32 t[3]; } MATRIX;
typedef struct { s32 vx, vy, vz, pad; } VECTOR;
typedef struct { s16 vx, vy, vz, pad; } SVECTOR;

/* ================================================================== libapi / libetc */
void *port_vsync_event_fn;          /* OpenEvent(RCntCNT3 / VBlank, ...) handler: the game's vblankHandler (set in card.c) */
static void (*vsync_cb)(void);
int port_frame;
int port_max_frames;
static unsigned long long next_vblank_us;
int port_realtime = 0;               /* 0 = run as fast as possible (headless), 1 = paced at port_hz */
int port_hz = 50;                    /* vblanks per second when paced: 50 (PAL, default) or 60 (--hz 60) */

static void call_psx(void *fn) {
    void (*f)(void) = (void (*)(void))fn;
    if ((u32)fn >= 0x80000000u && (u32)fn < 0x80200000u) f = (void (*)(void))port_resolve_call((u32)fn, "event handler");
    f();
}

/* --poke FRAME:ADDR:VALUE[:SIZE]: write RAM at the start of vblank FRAME (test setups, e.g. the debug area select
   flag 0x1F8001B4); ADDR and VALUE in hex, SIZE 1, 2 or 4 bytes (default 1).
   --poke-every INTERVAL:ADDR:VALUE[:SIZE]: the same every INTERVAL vblanks (mods: infinite lives / vitality);
   stored as a negative frame. */
static struct { int frame, size, count; u32 addr, value; } pokes[32];
static int n_pokes;
static u32 hexval(const char **p) {
    u32 v = 0; const char *h = *p;
    if (h[0] == '0' && (h[1] | 32) == 'x') h += 2;
    for (; (*h >= '0' && *h <= '9') || ((*h | 32) >= 'a' && (*h | 32) <= 'f'); h++)
        v = v * 16 + (*h <= '9' ? *h - '0' : (*h | 32) - 'a' + 10);
    *p = h; return v;
}
void port_poke_add_every(const char *s, int every) {
    if (n_pokes == 32) return;
    int f = 0;
    while (*s >= '0' && *s <= '9') f = f * 10 + *s++ - '0';
    if (*s++ != ':') return;
    pokes[n_pokes].frame = every ? -(f > 0 ? f : 1) : f;
    pokes[n_pokes].addr = hexval(&s);
    if (*s++ != ':') return;
    pokes[n_pokes].value = hexval(&s);
    pokes[n_pokes].size = *s == ':' ? s[1] - '0' : 1;
    pokes[n_pokes].count = 1;
    if (*s == ':') { s += 2; if (*s == 'x') { int c = 0; s++; while (*s >= '0' && *s <= '9') c = c * 10 + *s++ - '0'; if (c > 0) pokes[n_pokes].count = c; } }
    n_pokes++;
}
void port_poke_add(const char *s) { port_poke_add_every(s, 0); }
/* --dump-ram FRAME:PATH: write the 2 MB of RAM, then the 1 KB scratchpad, at the start of vblank FRAME */
static int dump_ram_frame; static const char *dump_ram_path;
void port_dump_ram_add(const char *s) {
    int f = 0;
    while (*s >= '0' && *s <= '9') f = f * 10 + *s++ - '0';
    if (*s++ != ':') return;
    dump_ram_frame = f; dump_ram_path = s;
}
static void dump_ram(void) {
    extern int port_create(const char *); extern int port_write(int, const void *, int); extern void port_close(int);
    int fd = port_create(dump_ram_path);
    if (fd < 0) { port_printf("[port] cannot write %s\n", dump_ram_path); return; }
    for (u32 a = 0; a < 0x200000; a += 0x10000) port_write(fd, (const void *)(0x80000000u + a), 0x10000);
    port_write(fd, (const void *)0x1F800000u, 0x400);
    port_close(fd);
}
/* save states (runtime/state.c): host-side values that a restored image must not change */
void port_realtime_reset(void) { next_vblank_us = 0; }
/* --menu FRAME:N (debug): open the in-game menu screen N at that vblank as the save point / pause menu would
   (4 = save, 5 = load), if the game task is in plain gameplay then */
static int menu_frame, menu_id;
void port_menu_add(const char *s) {
    int f = 0;
    while (*s >= '0' && *s <= '9') f = f * 10 + *s++ - '0';
    if (*s++ != ':') return;
    menu_frame = f; menu_id = 0;
    while (*s >= '0' && *s <= '9') menu_id = menu_id * 10 + *s++ - '0';
}
static void menu_open(void) {
    extern void openMenuScreen(short), gameMainTask(void);
    u32 task = 0;
    for (int i = 0; i < 3; i++) {                   /* the task table: the entry running gameMainTask */
        u8 *t = (u8 *)(0x801FD800u + i * 0x70);
        u32 entry = *(u32 *)(t + 0xC);
        if (entry == (u32)gameMainTask || port_lookup(entry) == (void *)gameMainTask) { task = (u32)t; break; }
    }
    if (!task) { port_printf("[port] --menu: gameMainTask is not running\n"); return; }
    u16 *w = (u16 *)(task + 0x48);
    if (w[0] != 1 || w[1] != 1 || w[2] == 3) { port_printf("[port] --menu: task state %d/%d/%d is not gameplay, skipped\n", w[0], w[1], w[2]); return; }
    u32 saved = *(u32 *)0x1F8001D4u;                /* TASK_C: openMenuScreen works on the current task */
    *(u32 *)0x1F8001D4u = task;
    openMenuScreen((short)menu_id);
    *(u32 *)0x1F8001D4u = saved;
    port_printf("[port] frame %d: menu screen %d opened\n", port_frame, menu_id);
}
void port_poke_reset(void) { n_pokes = 0; dump_ram_path = 0; menu_frame = 0; }
int port_cd_fd_get(void);
void port_cd_fd_set(int fd);
void port_vblank(void) {
    { extern void port_state_vblank(void); port_state_vblank(); }
    if (port_realtime) {
        unsigned long long now = port_time_us();
        if (!next_vblank_us) next_vblank_us = now;
        next_vblank_us += 1000000ull / (unsigned)port_hz;   /* PAL: 50 Hz (20 ms); --hz 60: 16.67 ms */
        if (next_vblank_us > now) port_sleep_us(next_vblank_us - now);
    }
    port_frame++;
    for (int i = 0; i < n_pokes; i++)
        if (pokes[i].frame == port_frame || (pokes[i].frame < 0 && port_frame % -pokes[i].frame == 0)) {
            u32 a = pokes[i].addr, v = pokes[i].value;
            for (int k = 0; k < pokes[i].count; k++, a += pokes[i].size)   /* SIZExCOUNT: consecutive values */
                if (pokes[i].size == 4) *(u32 *)a = v; else if (pokes[i].size == 2) *(u16 *)a = v; else *(u8 *)a = v;
        }
    if (dump_ram_path && port_frame == dump_ram_frame) dump_ram();
    if (menu_frame && port_frame == menu_frame) menu_open();
    {   /* FIFOs: open the video pipe, then the audio pipe, before anything is written (a player such as ffmpeg
           opens its inputs in that order and would wait forever on the second one) */
        extern void port_pipe_video_open(void), port_pipe_audio_open(void);
        static int opened;
        if (!opened) { opened = 1; port_pipe_video_open(); port_pipe_audio_open(); }
    }
    { extern void port_pad_vblank(void); port_pad_vblank(); }
    if (port_vsync_event_fn) call_psx(port_vsync_event_fn);
    if (vsync_cb) call_psx((void *)vsync_cb);
    /* sound first: with both pipes on, a player that interleaves audio and video by timestamp finds the
       audio of this vblank already buffered while it reads the (larger) video frame */
    { extern void port_sound_vblank(void); port_sound_vblank(); }
    port_gpu_vblank();
    {   /* -v: report area / section changes and (every 50 frames) where the player is */
        static int last = -1;
        int cur = *(u16 *)0x8009C960u << 16 | *(u16 *)0x8009C962u;
        u32 hp = *(u32 *)(0x800A6038u + 0x40);
        int px = (hp >= 0x80000000u && hp < 0x80200000u) ? *(s16 *)(hp + 2) : -1;
        int py = *(s16 *)(0x800A6038u + 0x16);
        if (port_verbose && (cur != last || port_verbose >= 3 || port_frame % (port_verbose >= 2 ? 10 : 50) == 0))
            port_printf("[port] frame %d: area %d section %d, player at %d,%d%s", port_frame, cur >> 16, cur & 0xFFFF,
                        px, py, cur != last ? " (changed)" : "");
        if (port_verbose >= 3) {   /* -v -v -v: mode.step.state, air state (ac), velY, and the held/bitten object */
            u8 *pl = (u8 *)0x800A6038u;
            u32 held = *(u32 *)(pl + 0xE4);
            port_printf(" %d.%d.%d ac=%d vy=%d", pl[4], pl[5], pl[6], pl[0xAC], *(s16 *)(pl + 0x7E));
            if (pl[0xAC] == 2 && held >= 0x80000000u && held < 0x80200000u) {
                u32 hh = *(u32 *)(held + 0x40);
                port_printf(" held=%02x@%d,%d", *(u8 *)(held + 2),
                            (hh >= 0x80000000u && hh < 0x80200000u) ? *(s16 *)(hh + 2) : -1, *(s16 *)(held + 0x16));
            }
        }
        if (port_verbose >= 3) {   /* ... and the active category-2 type-2 objects (AREA00: koma pigs): x,y mode.step.state ac */
            u32 lst = 0x800A6610u;   /* the layer list (initLayer1DrawList): entries grow downwards from the base */
            for (int i = 1; i < 64; i++) {
                u32 q = *(u32 *)(lst - 4 * i);
                if (q < 0x80000000u || q >= 0x80200000u) break;
                u8 *b = (u8 *)q; u32 qh = *(u32 *)(b + 0x40);
                if ((b[0] & 1) && b[2] == 2)
                    port_printf(" pig@%d,%d %d.%d.%d ac=%d", (qh >= 0x80000000u && qh < 0x80200000u) ? *(s16 *)(qh + 2) : -1,
                                *(s16 *)(b + 0x16), b[4], b[5], b[6], b[0xAC]);
            }
        }
        if (port_verbose && (cur != last || port_verbose >= 3 || port_frame % (port_verbose >= 2 ? 10 : 50) == 0))
            port_printf("\n");
        last = cur;
    }
    if (port_frame % 100 == 0) {
        extern int port_cd_stats(int *);
        extern int port_gpu_lists(void), port_gpu_packets(void);
        int secs, reads = port_cd_stats(&secs);
        port_printf("[port] frame %d: %d CD reads (%d sectors), %d ordering tables drawn (%d packets); tasks:",
                    port_frame, reads, secs, port_gpu_lists(), port_gpu_packets());
        for (int i = 0; i < 3; i++) {
            u8 *t = (u8 *)(0x801FD800u + i * 0x70);
            u32 entry = *(u32 *)(t + 0xC);
            const char *nm = "-";
            for (int k = 0; k < port_funcmap_count; k++)
                if (port_funcmap[k].fn == (void *)entry || port_funcmap[k].addr == entry) { nm = port_funcmap[k].name; break; }
            port_printf(" [%d st=%d %s step=%d.%d]", i, *(s16 *)t, nm, *(u16 *)(t + 0x48), *(u16 *)(t + 0x4A));
        }
        port_printf(" gameState=%02x\n", *(u8 *)0x1F8001CC);
    }
    if (port_max_frames && port_frame >= port_max_frames) {
        extern void port_stub_report(void);
        port_printf("[port] reached %d frames, stopping\n", port_frame);
        port_stub_report();
        { extern void port_check_report(void); port_check_report(); }
        { extern void port_audio_close(void); port_audio_close(); }
        port_exit(0);
    }
}

int EnterCriticalSection(void) { return 1; }
void ExitCriticalSection(void) {}
/* OpenEvent/CloseEvent/TestEvent/DeliverEvent: shims/card.c (the memory card is the only event source) */
int EnableEvent(int ev) { return 1; }
int DisableEvent(int ev) { return 1; }
int WaitEvent(int ev) { return 1; }
int OpenTh(void *fn, u32 sp, u32 gp) { return port_thread_open(fn); }
int ChangeTh(int h) { port_thread_change(h); return 1; }
int CloseTh(int h) { port_thread_close(h); return 1; }
void FlushCache(void) {}
u32 GetGp(void) { return 0; }
void SysEnqIntRP(int p, void *q) {}
void SysDeqIntRP(int p, void *q) {}
int ChangeClearRCnt(int t, int m) { return 0; }
int ChangeClearPAD(int v) { return 0; }
int SetVideoMode(int m) { return 1; /* PAL */ }
int GetVideoMode(void) { return 1; }
void InitHeap(void *p, u32 n) {}
int ResetCallback(void) { return 0; }
int StopCallback(void) { return 0; }
int RestartCallback(void) { return 0; }
int VSyncCallback(void (*f)(void)) { vsync_cb = f; return 0; }
int VSyncCallbacks(int n, void (*f)(void)) { if (n == 0) vsync_cb = f; return 0; }
int DMACallback(int ch, void *f) { return 0; }
int InterruptCallback(int n, void *f) { return 0; }
int CdRemove(void) { return 0; }
int _padChkVsync(void) { return 0; }
void FUN_8006bac4(void) {}
void _96_remove(void) {}
void __main(void) {}   /* crt0 constructor hook: nothing to run */

int VSync(int mode) {
    static int last;
    if (mode < 0) return port_frame;
    if (mode == 1) { int r = port_frame - last; return r; }
    int n = mode == 0 ? 1 : mode;
    for (int i = 0; i < n; i++) port_vblank();
    int r = port_frame - last; last = port_frame;
    return r;
}

/* memory card: shims/card.c */

/* ================================================================== libgpu: see gpu.c */

/* ================================================================== libgte */
void port_gte_ctc2(int r, unsigned long v);
unsigned long port_gte_cfc2(int r);
void port_gte_mtc2(int r, unsigned long v);
unsigned long port_gte_mfc2(int r);
void port_gte_op(unsigned long op);
#define PK(a, b) (((u32)(u16)(a)) | ((u32)(u16)(b) << 16))
static void load_matrix(int base, MATRIX *m) {
    port_gte_ctc2(base, PK(m->m[0][0], m->m[0][1])); port_gte_ctc2(base + 1, PK(m->m[0][2], m->m[1][0]));
    port_gte_ctc2(base + 2, PK(m->m[1][1], m->m[1][2])); port_gte_ctc2(base + 3, PK(m->m[2][0], m->m[2][1]));
    port_gte_ctc2(base + 4, (u16)m->m[2][2]);
}
void InitGeom(void) { port_gte_ctc2(29, 0x155); port_gte_ctc2(30, 0x100); port_gte_ctc2(26, 1000);
                      port_gte_ctc2(27, -0x1062); port_gte_ctc2(28, 0x1400000); }
void SetGeomScreen(int h) { port_gte_ctc2(26, h); }
void SetGeomOffset(int x, int y) { port_gte_ctc2(24, x << 16); port_gte_ctc2(25, y << 16); }
void SetRotMatrix(MATRIX *m) { load_matrix(0, m); }
void SetLightMatrix(MATRIX *m) { load_matrix(8, m); }
void SetColorMatrix(MATRIX *m) { load_matrix(16, m); }
void SetTransMatrix(MATRIX *m) { port_gte_ctc2(5, m->t[0]); port_gte_ctc2(6, m->t[1]); port_gte_ctc2(7, m->t[2]); }
void SetBackColor(int r, int g, int b) { port_gte_ctc2(13, r << 4); port_gte_ctc2(14, g << 4); port_gte_ctc2(15, b << 4); }
void SetFarColor(int r, int g, int b) { port_gte_ctc2(21, r << 4); port_gte_ctc2(22, g << 4); port_gte_ctc2(23, b << 4); }

/* rsin/rcos: 4096 = 1.0 over a 4096-step circle; table built once with a Taylor series (no libm) */
static s16 sintab[1025];
static void sin_init(void) {
    if (sintab[1024]) return;
    for (int i = 0; i <= 1024; i++) {
        double x = i * (3.14159265358979323846 / 2048.0), t = x, s = x;
        for (int k = 1; k < 12; k++) { t *= -x * x / ((2 * k) * (2 * k + 1)); s += t; }
        sintab[i] = (s16)(s * 4096.0 + 0.5);
    }
}
int rsin(int a) {
    sin_init();
    a &= 4095;
    if (a < 1024) return sintab[a];
    if (a < 2048) return sintab[2048 - a];
    if (a < 3072) return -sintab[a - 2048];
    return -sintab[4096 - a];
}
int rcos(int a) { return rsin(a + 1024); }
int ratan2(int y, int x) {
    /* binary search on the angle: monotone in the first octant */
    int neg_y = y < 0, neg_x = x < 0, swap;
    unsigned ay = neg_y ? -y : y, ax = neg_x ? -x : x;
    if (!ax && !ay) return 0;
    swap = ay > ax;
    unsigned num = swap ? ax : ay, den = swap ? ay : ax;        /* ratio <= 1 */
    int lo = 0, hi = 512;                                        /* 0..45 degrees */
    while (lo < hi) {
        int m = (lo + hi + 1) / 2;
        if ((long long)rsin(m) * den <= (long long)rcos(m) * num) lo = m; else hi = m - 1;
    }
    int a = swap ? 1024 - lo : lo;
    if (neg_x) a = 2048 - a;
    if (neg_y) a = -a;
    return a;
}
static u32 isqrt(u32 v) { u32 r = 0, b = 1u << 30; while (b > v) b >>= 2;
    while (b) { if (v >= r + b) { v -= r + b; r = (r >> 1) + b; } else r >>= 1; b >>= 2; } return r; }
int SquareRoot0(int v) { return isqrt((u32)v); }
int csqrt(int v) { return (int)isqrt((u32)v << 12 >> 0) ; }  /* fixed 20.12 */

MATRIX *RotMatrixX(int r, MATRIX *m) {
    int s = rsin(r), c = rcos(r);
    for (int i = 0; i < 3; i++) {
        int a = m->m[1][i], b = m->m[2][i];
        m->m[1][i] = (c * a - s * b) >> 12; m->m[2][i] = (s * a + c * b) >> 12;
    }
    return m;
}
MATRIX *RotMatrixY(int r, MATRIX *m) {
    int s = rsin(r), c = rcos(r);
    for (int i = 0; i < 3; i++) {
        int a = m->m[0][i], b = m->m[2][i];
        m->m[0][i] = (c * a + s * b) >> 12; m->m[2][i] = (-s * a + c * b) >> 12;
    }
    return m;
}
MATRIX *RotMatrixZ(int r, MATRIX *m) {
    int s = rsin(r), c = rcos(r);
    for (int i = 0; i < 3; i++) {
        int a = m->m[0][i], b = m->m[1][i];
        m->m[0][i] = (c * a - s * b) >> 12; m->m[1][i] = (s * a + c * b) >> 12;
    }
    return m;
}
MATRIX *RotMatrix(SVECTOR *r, MATRIX *m) {
    /* Psy-Q RotMatrix: M = Rx * Ry * Rz applied as rotate Z, then Y, then X */
    int sx = rsin(r->vx), cx = rcos(r->vx), sy = rsin(r->vy), cy = rcos(r->vy), sz = rsin(r->vz), cz = rcos(r->vz);
    m->m[0][0] = (cy * cz) >> 12;
    m->m[0][1] = -(cy * sz) >> 12;
    m->m[0][2] = sy;
    m->m[1][0] = ((cx * sz) >> 12) + (((sx * sy) >> 12) * cz >> 12);
    m->m[1][1] = ((cx * cz) >> 12) - (((sx * sy) >> 12) * sz >> 12);
    m->m[1][2] = -(sx * cy) >> 12;
    m->m[2][0] = ((sx * sz) >> 12) - (((cx * sy) >> 12) * cz >> 12);
    m->m[2][1] = ((sx * cz) >> 12) + (((cx * sy) >> 12) * sz >> 12);
    m->m[2][2] = (cx * cy) >> 12;
    return m;
}
MATRIX *MulMatrix0(MATRIX *a, MATRIX *b, MATRIX *c) {
    MATRIX t;
    for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++)
        t.m[i][j] = (a->m[i][0] * b->m[0][j] + a->m[i][1] * b->m[1][j] + a->m[i][2] * b->m[2][j]) >> 12;
    for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) c->m[i][j] = t.m[i][j];
    return c;
}
VECTOR *ApplyMatrix(MATRIX *m, SVECTOR *v, VECTOR *r) {
    s32 x = v->vx, y = v->vy, z = v->vz;
    r->vx = (m->m[0][0] * x + m->m[0][1] * y + m->m[0][2] * z) >> 12;
    r->vy = (m->m[1][0] * x + m->m[1][1] * y + m->m[1][2] * z) >> 12;
    r->vz = (m->m[2][0] * x + m->m[2][1] * y + m->m[2][2] * z) >> 12;
    return r;
}
SVECTOR *ApplyMatrixSV(MATRIX *m, SVECTOR *v, SVECTOR *r) {
    VECTOR t; ApplyMatrix(m, v, &t); r->vx = t.vx; r->vy = t.vy; r->vz = t.vz; return r;
}
VECTOR *ApplyMatrixLV(MATRIX *m, VECTOR *v, VECTOR *r) {
    long long x = v->vx, y = v->vy, z = v->vz;
    r->vx = (s32)((m->m[0][0] * x + m->m[0][1] * y + m->m[0][2] * z) >> 12);
    r->vy = (s32)((m->m[1][0] * x + m->m[1][1] * y + m->m[1][2] * z) >> 12);
    r->vz = (s32)((m->m[2][0] * x + m->m[2][1] * y + m->m[2][2] * z) >> 12);
    return r;
}
VECTOR *ApplyRotMatrix(SVECTOR *v, VECTOR *r) {
    port_gte_mtc2(0, PK(v->vx, v->vy)); port_gte_mtc2(1, (u16)v->vz);
    port_gte_op(0x00486012);   /* MVMVA sf=1 mx=RT v=V0 cv=none */
    r->vx = (s32)port_gte_mfc2(25); r->vy = (s32)port_gte_mfc2(26); r->vz = (s32)port_gte_mfc2(27);
    return r;
}
int RotTransPers(SVECTOR *v, s32 *sxy, s32 *p, s32 *flag) {
    port_gte_mtc2(0, PK(v->vx, v->vy)); port_gte_mtc2(1, (u16)v->vz);
    port_gte_op(0x00180001);
    *sxy = (s32)port_gte_mfc2(14); *p = (s32)port_gte_mfc2(8); *flag = (s32)port_gte_cfc2(31);
    return (s32)port_gte_mfc2(19) >> 2;
}

/* ================================================================== libcd (reads the user's disc image) */
static int cd_fd = -1, cd_raw;
int port_cd_fd_get(void) { return cd_fd; }
void port_cd_fd_set(int fd) { cd_fd = fd; }
static int cd_pos;                     /* next LBA */
static void *cd_ready_cb, *cd_sync_cb, *cd_read_cb;
static u8 cd_mode = 0x80;
/* A `.cue` names the data track's `.bin` (FILE "name" BINARY, relative to the cue): resolve it, else
   the path is the image itself. */
static char cd_bin_path[1024];
static const char *cd_resolve_cue(const char *path) {
    int n = 0; while (path[n]) n++;
    if (n < 4 || (path[n - 4] != '.') || ((path[n - 3] | 32) != 'c') || ((path[n - 2] | 32) != 'u') ||
        ((path[n - 1] | 32) != 'e')) return path;
    int fd = port_open(path);
    if (fd < 0) return path;
    static char cue[8192];
    int len = port_read(fd, cue, sizeof cue - 1);
    port_close(fd);
    if (len < 0) len = 0;
    cue[len] = 0;
    /* first FILE "..." line (the data track is track 1) */
    for (int i = 0; i + 5 < len; i++) {
        if ((i == 0 || cue[i - 1] == '\n' || cue[i - 1] == ' ' || cue[i - 1] == '\t') &&
            (cue[i] | 32) == 'f' && (cue[i + 1] | 32) == 'i' && (cue[i + 2] | 32) == 'l' && (cue[i + 3] | 32) == 'e' &&
            (cue[i + 4] == ' ' || cue[i + 4] == '\t')) {
            int j = i + 5; while (cue[j] == ' ' || cue[j] == '\t') j++;
            char name[512]; int k = 0;
            if (cue[j] == '"') { j++; while (cue[j] && cue[j] != '"' && k < 511) name[k++] = cue[j++]; }
            else while (cue[j] && cue[j] != ' ' && cue[j] != '\t' && cue[j] != '\r' && cue[j] != '\n' && k < 511) name[k++] = cue[j++];
            name[k] = 0;
            if (!k) break;
            int dir = n; while (dir > 0 && path[dir - 1] != '/' && path[dir - 1] != '\\') dir--;    /* directory of the cue */
            int o = 0;
            int absolute = name[0] == '/' || name[0] == '\\' || (name[0] && name[1] == ':');   /* C:\... on Windows */
            if (!absolute) for (int q = 0; q < dir && o < 1000; q++) cd_bin_path[o++] = path[q];
            for (int q = 0; name[q] && o < 1020; q++) cd_bin_path[o++] = name[q];
            cd_bin_path[o] = 0;
            port_printf("[port] cue sheet %s -> %s\n", path, cd_bin_path);
            return cd_bin_path;
        }
    }
    port_printf("[port] %s: no FILE line found in the cue sheet\n", path);
    return path;
}
static const char *cd_image_path = "(none)";
static long long cd_image_size;
static int cd_sector_count;
int port_trace_cd;                     /* --trace-cd: every Setloc and read */
/* The sector size comes from the image itself: a raw (2352) image starts with the 12-byte sync
   00 FF*10 00, a cooked (2048) ISO has the primary volume descriptor "\1CD001" at 16*2048. The file size
   alone is ambiguous (an image with a trailing audio track or a cue-less multi-track dump is not a multiple
   of 2352 and was read as 2048 before, which gives garbage directories and bogus load records). */
int port_cd_open(const char *path) {
    path = cd_resolve_cue(path);
    cd_fd = port_open(path);
    if (cd_fd < 0) return -1;
    cd_image_path = path;
    long long n = cd_image_size = port_filesize(cd_fd);
    u8 head[16] = { 0 }, pvd[8] = { 0 };
    port_pread(cd_fd, head, 16, 0);
    static const u8 sync[12] = { 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0 };
    int has_sync = 1;
    for (int i = 0; i < 12; i++) if (head[i] != sync[i]) has_sync = 0;
    int has_pvd = port_pread(cd_fd, pvd, 8, 16 * 2048) == 8 && pvd[1] == 'C' && pvd[2] == 'D' && pvd[3] == '0' && pvd[4] == '0' && pvd[5] == '1';
    /* sizes stay below 2^32 (a CD is at most ~800 MB); the freestanding runtime has no 64-bit division helper */
    unsigned size32 = n > 0xFFFFFFFFll ? 0xFFFFFFFFu : (unsigned)n;
    unsigned rest = size32 % 2352;
    cd_raw = has_sync ? 1 : has_pvd ? 0 : rest == 0;
    cd_sector_count = (int)(size32 / (cd_raw ? 2352 : 2048));
    port_printf("[port] disc image %s: %lld bytes, %s sectors (%d), %s\n", path, n, cd_raw ? "raw 2352" : "cooked 2048",
                cd_sector_count, has_sync ? "sync pattern found" : has_pvd ? "ISO9660 PVD at 16*2048" : cd_raw ? "no sync pattern: size is a multiple of 2352" : "no sync pattern, no PVD: assumed cooked");
    if (cd_raw && rest) port_printf("[port] warning: %u trailing bytes beyond the last raw sector (several tracks in one file?)\n", rest);
    /* self-check: sector 16 is the primary volume descriptor */
    u8 sec[2048];
    if (port_cd_read_sectors(16, 1, sec, 2048) || sec[0] != 1 || sec[1] != 'C' || sec[2] != 'D' || sec[3] != '0' || sec[4] != '0' || sec[5] != '1')
        port_printf("[port] warning: sector 16 of %s is not an ISO9660 primary volume descriptor (%02x %02x %02x %02x %02x %02x): "
                    "wrong sector size or not a data track image\n", path, sec[0], sec[1], sec[2], sec[3], sec[4], sec[5]);
    else if (port_verbose || port_trace_cd) port_printf("[port] disc self-check: PVD ok, volume \"%.32s\"\n", sec + 40);
    return 0;
}
int port_cd_read_sectors(int lba, int count, void *dst, int bytes) {
    u8 *d = dst;
    if (cd_fd < 0) { port_printf("[port] CD read of lba %d: no disc image open\n", lba); return -1; }
    for (int i = 0; i < count; i++) {
        long long off = cd_raw ? (long long)(lba + i) * 2352 + (bytes == 2048 ? 24 : 12) : (long long)(lba + i) * 2048;
        int got = port_pread(cd_fd, d, bytes, off);
        if (port_trace_cd) port_printf("[port] cd: lba %d (%d/%d) %d bytes at offset %lld -> %d%s\n", lba + i, i + 1, count, bytes, off, got,
                                       got == bytes ? "" : " FAILED");
        if (got != bytes) {
            port_printf("[port] CD read failed: lba %d, %d bytes at offset %lld of %s (%lld bytes = %d sectors of %d), got %d (os error %d)%s\n",
                        lba + i, bytes, off, cd_image_path, cd_image_size, cd_sector_count, cd_raw ? 2352 : 2048, got, port_last_error(),
                        lba + i >= cd_sector_count ? ": beyond the end of the image" : "");
            return -1;
        }
        d += bytes;
    }
    return 0;
}
static int bcd(u8 b) { return (b >> 4) * 10 + (b & 15); }
static int loc_to_lba(const u8 *p) { return (bcd(p[0]) * 60 + bcd(p[1])) * 75 + bcd(p[2]) - 150; }
int CdInit(void) { return 1; }
int CdReset(int m) { return 1; }
int CdStatus(void) { return 0x02; }
int CdMode(void) { return cd_mode; }
/* Setloc parameters come from FILE_LINKS (cdLoadTask: &FILE_LINKS + id * 8, the id of a load record): a position
   beyond the image means a garbage record (on Linux the game never asks past lba 178339), so say where the
   parameter lives. */
extern u8 FILE_LINKS[];
static int cd_bad_locs;
static void cd_check_loc(const u8 *param, int lba) {
    unsigned off = (unsigned)(param - FILE_LINKS);
    int in_table = off < 1119 * 8 && (off & 7) == 0;
    if (port_trace_cd) port_printf("[port] cd: Setloc %02x:%02x:%02x -> lba %d (%s %u)\n", param[0], param[1], param[2], lba,
                                   in_table ? "FILE_LINKS id" : "param at", in_table ? off / 8 : (unsigned)(unsigned long)param);
    if (lba >= 0 && lba < cd_sector_count) return;
    if (cd_bad_locs++ > 20) return;
    port_printf("[port] CD Setloc %02x:%02x:%02x = lba %d is outside the image (%d sectors): ", param[0], param[1], param[2], lba, cd_sector_count);
    if (in_table) port_printf("FILE_LINKS[%u] (a load record with that file id is garbage: KUSEG/page-0 reads?)\n", off / 8);
    else port_printf("the parameter is at %p, not in FILE_LINKS (a corrupt CdlLOC)\n", param);
}
int CdControl(u8 com, u8 *param, u8 *result) {
    if ((com == 2 /* Setloc */ || com == 0x15 || com == 0x16) && param) { cd_pos = loc_to_lba(param); cd_check_loc(param, cd_pos); }
    if (com == 14 /* Setmode */ && param) cd_mode = param[0];
    if (result) result[0] = 0x02;
    return 1;
}
int CdControlF(u8 com, u8 *param) { return CdControl(com, param, 0); }
int CdControlB(u8 com, u8 *param, u8 *result) { return CdControl(com, param, result); }
int CdSync(int mode, u8 *result) { if (result) result[0] = 0x02; return 2; /* CdlComplete */ }
int CdReady(int mode, u8 *result) { return 2; }
void *CdSyncCallback(void *f) { void *o = cd_sync_cb; cd_sync_cb = f; return o; }
void *CdReadyCallback(void *f) { void *o = cd_ready_cb; cd_ready_cb = f; return o; }
void *CdReadCallback(void *f) { void *o = cd_read_cb; cd_read_cb = f; return o; }
static int cd_reads, cd_sectors;
int CdRead(int sectors, u32 *buf, int mode) {
    int bytes = (mode & 0x20) ? 2340 : 2048;
    if (port_verbose || port_trace_cd) port_printf("[port] CdRead lba %d, %d sectors -> %p (frame %d)\n", cd_pos, sectors, buf, port_frame);
    if (port_cd_read_sectors(cd_pos, sectors, buf, bytes)) { port_printf("[port] CdRead failed at lba %d (frame %d)\n", cd_pos, port_frame); return 0; }
    cd_pos += sectors; cd_reads++; cd_sectors += sectors;
    return 1;
}
int CdReadSync(int mode, u8 *result) { return 0; }
int CdReadBreak(void) { return 0; }
int CdMix(void *att) { return 1; }
int port_cd_stats(int *sectors) { *sectors = cd_sectors; return cd_reads; }
/* streaming (movies): not supported yet; CdRead2 "starts" a stream that never delivers a frame */
int CdRead2(int mode) { return 1; }
void StSetRing(void *a, int b) {}
void StClearRing(void) {}
void StUnSetRing(void) {}
int StFreeRing(void *a) { return 0; }
/* Movies are skipped (no MDEC yet): the first StGetNext hands MovieDecodeFrame a sector header whose frame
   number is past the end of every movie, so moviePlayerTask takes its own "movie finished" path (state 3:
   CdMix, StUnSetRing, exitTask) and the boot continues.  --play-movies keeps the old behaviour (waits forever). */
int port_play_movies;
static struct { u16 id, type, secCount, nSectors; u32 frameCount, frameSize; u16 width, height; } fake_st_header;
static u32 fake_st_frame[16];
int StGetNext(void **a, void **b) {
    if (port_play_movies) return 1;
    fake_st_header.frameCount = 0x7FFFFFFF;
    fake_st_header.width = 320; fake_st_header.height = 240;
    *a = fake_st_frame; *b = &fake_st_header;
    if (port_verbose) port_printf("[port] movie skipped (StGetNext reports the last frame)\n");
    return 0;
}
void StSetStream(int a, int b, int c, void *d, void *e) {}
/* MDEC (movies) */
void DecDCTReset(int m) {}
int DecDCTin(void *p, int m) { return 0; }
int DecDCTout(void *p, int n) { return 0; }
int DecDCTinSync(int m) { return 0; }
int DecDCToutSync(int m) { return 0; }
void *DecDCToutCallback(void *f) { return 0; }
int DecDCTvlc(void *a, void *b) { return 0; }

/* ================================================================== sound: silent for now */
void SpuQuit(void) {}
int SpuSetReverb(int on) { return on; }
