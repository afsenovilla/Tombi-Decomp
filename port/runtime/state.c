/* Save states: --save FRAME:PATH writes the whole game state at the start of vblank FRAME, --load PATH restores
 * one at the first vblank and continues from there (the restored vblank count carries on, so --pad-script,
 * --save, --dump-* frames of the new run count from the same origin as the saved run).
 *
 * The state is: PS1 RAM (2 MB), the scratchpad page, and every writable mapping of this static binary
 * (.data/.bss: the runtime's globals, the software GPU's VRAM, the SPU, the GTE, the coroutine stacks of the
 * game's tasks in threads.c).  The process stack is not saved: a state is taken and restored only from the
 * main loop's vblank wait (port_in_main_loop, main thread), where the stack holds the same frames in every run.
 * Host-side variables that must not come from the file (the run's options and open file descriptors) are
 * re-applied after the copy: the command line is parsed again and the fds listed in `keep` are preserved.
 * A state is only valid for the binary that wrote it (the layout of .data/.bss is checked by a hash). */
#include "port_runtime.h"

typedef unsigned char u8; typedef unsigned int u32;

#define MAX_SAVES 8
static struct { int frame; const char *path; int done; } saves[MAX_SAVES];
static int n_saves;
const char *port_load_path;
int port_in_main_loop;

extern int port_thread_current(void);
extern void port_reparse_options(void);
extern int port_write_all(int fd, const void *b, int n);
extern int port_cd_fd_get(void), port_window_fds_get(int *v, int *a, int *p);
extern void port_cd_fd_set(int), port_window_fds_set(int v, int a, int p);
extern void port_gpu_fds_get(int *win, int *pipe), port_gpu_fds_set(int win, int pipe);
extern int port_audio_pipe_fd_get(void); extern void port_audio_pipe_fd_set(int);
extern void port_realtime_reset(void);
extern int port_argc; extern char **port_argv;

void port_save_add(const char *s) {
    if (n_saves >= MAX_SAVES) port_fatal("--save: too many");
    int f = 0;
    while (*s >= '0' && *s <= '9') f = f * 10 + *s++ - '0';
    if (*s != ':') port_fatal("--save FRAME:PATH");
    saves[n_saves].frame = f; saves[n_saves].path = s + 1; saves[n_saves].done = 0; n_saves++;
}
void port_save_reset(void) { n_saves = 0; }

/* writable mappings of the binary from /proc/self/maps: "start-end perms ..." lines with rw and an address in
   [0x08000000, 0x1F800000): below that are the PS1 KUSEG mirrors, above it the scratchpad, RAM and the stack */
typedef struct { u32 start, end; } Region;
static int regions(Region *r, int max) {
    static char buf[16384];
    int fd = port_open("/proc/self/maps");
    if (fd < 0) port_fatal("cannot read /proc/self/maps");
    int n = 0, len = 0;
    for (;;) { int k = port_read(fd, buf + len, (int)sizeof buf - 1 - len); if (k <= 0) break; len += k; }
    port_close(fd);
    buf[len] = 0;
    const char *p = buf;
    while (*p && n < max) {
        u32 a = 0, b = 0;
        while (*p && *p != '-') { a = a * 16 + (*p <= '9' ? *p - '0' : (*p | 32) - 'a' + 10); p++; }
        if (*p == '-') p++;
        while (*p && *p != ' ') { b = b * 16 + (*p <= '9' ? *p - '0' : (*p | 32) - 'a' + 10); p++; }
        int rw = p[1] == 'r' && p[2] == 'w';
        while (*p && *p != '\n') p++;
        if (*p == '\n') p++;
        if (rw && a >= 0x08000000u && b <= 0x1F800000u) { r[n].start = a; r[n].end = b; n++; }
    }
    return n;
}

/* identity of this build: FNV-1a over the executable itself plus the region list (the layout of .data/.bss
   changes with almost any rebuild, and a mismatch restores variables at the wrong addresses) */
static u32 layout_hash(const Region *r, int n) {
    static u32 exe_hash;
    if (!exe_hash) {
        static u8 chunk[65536];
        u32 h = 2166136261u;
        int fd = port_open("/proc/self/exe");
        if (fd < 0) port_fatal("cannot read /proc/self/exe");
        for (;;) {
            int k = port_read(fd, chunk, sizeof chunk);
            if (k <= 0) break;
            for (int i = 0; i < k; i++) h = (h ^ chunk[i]) * 16777619u;
        }
        port_close(fd);
        exe_hash = h ? h : 1;
    }
    u32 h = exe_hash;
    for (int i = 0; i < n; i++) { h = (h ^ r[i].start) * 16777619u; h = (h ^ r[i].end) * 16777619u; }
    return h;
}

static void save_state(const char *path) {
    Region r[32]; int n = regions(r, 32);
    int fd = port_create(path);
    if (fd < 0) { port_printf("[port] --save: cannot create %s\n", path); return; }
    u32 hdr[4] = { 0x54534254u /* "TBST" */, 1, (u32)n + 2, layout_hash(r, n) };
    port_write_all(fd, hdr, 16);
    u32 reg[2];
    reg[0] = PSX_RAM_BASE; reg[1] = PSX_RAM_SIZE; port_write_all(fd, reg, 8); port_write_all(fd, (void *)PSX_RAM_BASE, PSX_RAM_SIZE);
    reg[0] = PSX_SPAD_BASE; reg[1] = PSX_SPAD_SIZE; port_write_all(fd, reg, 8); port_write_all(fd, (void *)PSX_SPAD_BASE, PSX_SPAD_SIZE);
    for (int i = 0; i < n; i++) {
        reg[0] = r[i].start; reg[1] = r[i].end - r[i].start;
        port_write_all(fd, reg, 8); port_write_all(fd, (void *)r[i].start, (int)reg[1]);
    }
    port_close(fd);
    port_printf("[port] frame %d: state saved to %s (%d regions)\n", port_frame, path, n + 2);
}

static void load_state(const char *path) {
    Region r[32]; int n = regions(r, 32);
    int fd = port_open(path);
    if (fd < 0) port_fatal("--load: cannot open %s", path);
    u32 hdr[4];
    if (port_read(fd, hdr, 16) != 16 || hdr[0] != 0x54534254u || hdr[1] != 1) port_fatal("--load: %s is not a state file", path);
    if (hdr[3] != layout_hash(r, n)) port_fatal("--load: %s was written by a different build of this binary", path);
    /* host-side values that the restored image must not change */
    int cd = port_cd_fd_get(), wv, wa, wp; port_window_fds_get(&wv, &wa, &wp);
    int gw, gp; port_gpu_fds_get(&gw, &gp);
    int ap = port_audio_pipe_fd_get();
    int argc = port_argc; char **argv = port_argv;
    for (u32 i = 0; i < hdr[2]; i++) {
        u32 reg[2];
        if (port_read(fd, reg, 8) != 8) port_fatal("--load: short file");
        u32 left = reg[1], a = reg[0];
        while (left) { int k = port_read(fd, (void *)a, (int)(left > 0x100000 ? 0x100000 : left)); if (k <= 0) port_fatal("--load: short file"); a += k; left -= k; }
    }
    port_close(fd);
    port_cd_fd_set(cd); port_window_fds_set(wv, wa, wp); port_gpu_fds_set(gw, gp); port_audio_pipe_fd_set(ap);
    port_argc = argc; port_argv = argv;
    port_in_main_loop = 1;
    port_realtime_reset();
    port_reparse_options();          /* this run's frames, scripts, pokes, dumps, pipes; clears port_load_path */
    port_printf("[port] state %s restored at frame %d\n", path, port_frame);
}

/* at the start of every vblank, before the frame counter advances */
void port_state_vblank(void) {
    if (!port_in_main_loop || port_thread_current() != 0) return;
    if (port_load_path) { const char *p = port_load_path; port_load_path = 0; load_state(p); }
    for (int i = 0; i < n_saves; i++)
        if (!saves[i].done && port_frame >= saves[i].frame) { saves[i].done = 1; save_state(saves[i].path); }
}
