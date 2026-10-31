/* Native host of the wasm2c-translated port2 (port2/wasm, the wasm32 spike): instantiates the module, serves its
 * imports and runs _start.
 *   - WASI preview1 subset (what wasi-libc's stdio, open/stat/mkdir, clock and nanosleep use), straight onto
 *     POSIX.  One preopen, "/" = the host root; the host's working directory goes in as PWD and rt_wasm.c chdirs
 *     there, so relative paths (data/, game/, --dump-frames out) mean what they mean for the PC build.
 *   - port.ctx_switch: the BIOS-thread coroutines of runtime/threads.c.  The translated functions run on the
 *     native stack, so a switch is a native stack switch (ucontext) plus the module's __stack_pointer global (the
 *     shadow stack in linear memory, one per coroutine: the top of its stacks[] slot in threads.c).
 * The module's linear memory holds every game pointer as a 32-bit offset, exactly as on the PS1 / the i386 build. */
#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <time.h>
#include <ucontext.h>
#include <unistd.h>
#include "tombi.h"

uint64_t port_w2c_low_base;          /* patch_w2c.py: the null-page window maps here (port_ram) */
static w2c_tombi inst;
#define MEM (inst.w2c_memory.data)
static inline uint8_t *P(u32 a) { return MEM + a; }
static inline void W32(u32 a, u32 v) { memcpy(P(a), &v, 4); }
static inline void W64(u32 a, u64 v) { memcpy(P(a), &v, 8); }
static inline void W16(u32 a, u16 v) { memcpy(P(a), &v, 2); }
static inline u32 R32(u32 a) { u32 v; memcpy(&v, P(a), 4); return v; }

/* ------------------------------------------------------------------------------------------- coroutines */
#define NCTX 16
#define NATIVE_STACK (8 << 20)
static struct { ucontext_t uc; u32 key, ssp; void *stack; } ctx[NCTX];
static int cur_ctx, nctx = 1;            /* [0] = the main thread */
static void co_entry(void) { w2c_tombi_port_thread_entry_wasm(&inst); fprintf(stderr, "[wasm] coroutine returned\n"); exit(3); }
void w2c_port_ctx_switch(struct w2c_port *unused, u32 save, u32 to) {
    (void)unused;
    int from = cur_ctx, n;
    u32 *sp = w2c_tombi_0x5F_stack_pointer(&inst);
    ctx[from].ssp = *sp;
    W32(save, 0x80000000u | from);
    if (to & 0x80000000u) n = to & 0x7FFFFFFF;
    else {                               /* a fresh coroutine: its shadow stack top; one host context per stack */
        for (n = 1; n < nctx && ctx[n].key != to; n++) ;
        if (n == nctx) { if (nctx == NCTX) { fprintf(stderr, "[wasm] out of contexts\n"); exit(3); } nctx++; ctx[n].key = to; ctx[n].stack = malloc(NATIVE_STACK); }
        getcontext(&ctx[n].uc);
        ctx[n].uc.uc_stack.ss_sp = ctx[n].stack; ctx[n].uc.uc_stack.ss_size = NATIVE_STACK; ctx[n].uc.uc_link = 0;
        makecontext(&ctx[n].uc, co_entry, 0);
        ctx[n].ssp = to;
    }
    cur_ctx = n;
    *sp = ctx[n].ssp;
    swapcontext(&ctx[from].uc, &ctx[n].uc);
}

/* ------------------------------------------------------------------------------------------- WASI */
static int g_argc; static char **g_argv; static char pwd_env[4200];
static int fdmap[256];                   /* wasm fd -> host fd (-1: closed); 3 = the "/" preopen */
static int werr(int e) {
    switch (e) { case 0: return 0; case EACCES: return 2; case EAGAIN: return 6; case EBADF: return 8; case EEXIST: return 20;
    case EINVAL: return 28; case EIO: return 29; case EISDIR: return 31; case ENOENT: return 44; case ENOSPC: return 51;
    case ENOTDIR: return 54; case ENOTEMPTY: return 55; case EPERM: return 63; case ESPIPE: return 70; default: return 29; }
}
static int hfd(u32 fd) { return fd < 256 ? fdmap[fd] : -1; }
static char *path_of(u32 p, u32 len) { static char buf[3][4200]; static int k; char *b = buf[k++ % 3];
    if (len > 4096) len = 4096; memcpy(b, P(p), len); b[len] = 0; return b; }
#define WASI(name) u32 w2c_wasi__snapshot__preview1_##name(struct w2c_wasi__snapshot__preview1 *u_
WASI(args_sizes_get), u32 argc, u32 bufsz) { size_t n = 0; for (int i = 0; i < g_argc; i++) n += strlen(g_argv[i]) + 1;
    W32(argc, g_argc); W32(bufsz, n); return 0; }
WASI(args_get), u32 argv, u32 buf) { for (int i = 0; i < g_argc; i++) { size_t l = strlen(g_argv[i]) + 1; memcpy(P(buf), g_argv[i], l);
    W32(argv + 4 * i, buf); buf += l; } return 0; }
WASI(environ_sizes_get), u32 cnt, u32 bufsz) { W32(cnt, 1); W32(bufsz, strlen(pwd_env) + 1); return 0; }
WASI(environ_get), u32 env, u32 buf) { memcpy(P(buf), pwd_env, strlen(pwd_env) + 1); W32(env, buf); return 0; }
WASI(clock_time_get), u32 id, u64 prec, u32 out) { struct timespec ts; (void)prec;
    clock_gettime(id == 0 ? CLOCK_REALTIME : CLOCK_MONOTONIC, &ts); W64(out, (u64)ts.tv_sec * 1000000000ull + ts.tv_nsec); return 0; }
WASI(fd_close), u32 fd) { int h = hfd(fd); if (h < 0 || fd <= 3) return 8; close(h); fdmap[fd] = -1; return 0; }
WASI(fd_fdstat_get), u32 fd, u32 buf) { int h = hfd(fd); struct stat st; if (h < 0 || fstat(h, &st)) return 8;
    memset(P(buf), 0, 24); P(buf)[0] = S_ISDIR(st.st_mode) ? 3 : S_ISREG(st.st_mode) ? 4 : 2; W64(buf + 8, ~0ull); W64(buf + 16, ~0ull); return 0; }
WASI(fd_fdstat_set_flags), u32 fd, u32 fl) { (void)fd; (void)fl; return 0; }
WASI(fd_prestat_get), u32 fd, u32 buf) { if (fd != 3) return 8; W32(buf, 0); W32(buf + 4, 1); return 0; }
WASI(fd_prestat_dir_name), u32 fd, u32 p, u32 len) { if (fd != 3 || len < 1) return 8; P(p)[0] = '/'; return 0; }
static u32 rw(u32 fd, u32 iovs, u32 n, u32 out, int wr) {
    int h = hfd(fd); if (h < 0) return 8;
    struct iovec v[64]; if (n > 64) n = 64;
    for (u32 i = 0; i < n; i++) { v[i].iov_base = P(R32(iovs + 8 * i)); v[i].iov_len = R32(iovs + 8 * i + 4); }
    ssize_t r = wr ? writev(h, v, n) : readv(h, v, n);
    if (r < 0) return werr(errno); W32(out, (u32)r); return 0;
}
WASI(fd_read), u32 fd, u32 iovs, u32 n, u32 out) { return rw(fd, iovs, n, out, 0); }
WASI(fd_write), u32 fd, u32 iovs, u32 n, u32 out) { return rw(fd, iovs, n, out, 1); }
WASI(fd_seek), u32 fd, u64 off, u32 whence, u32 out) { int h = hfd(fd); if (h < 0) return 8;
    off_t r = lseek(h, (off_t)(int64_t)off, whence == 0 ? SEEK_SET : whence == 1 ? SEEK_CUR : SEEK_END);
    if (r < 0) return werr(errno); W64(out, (u64)r); return 0; }
WASI(fd_readdir), u32 fd, u32 buf, u32 len, u64 cookie, u32 out) { (void)fd; (void)buf; (void)len; (void)cookie; W32(out, 0); return 52; }
static void put_filestat(u32 buf, struct stat *st) {
    memset(P(buf), 0, 64); W64(buf, st->st_dev); W64(buf + 8, st->st_ino);
    P(buf)[16] = S_ISDIR(st->st_mode) ? 3 : S_ISREG(st->st_mode) ? 4 : S_ISLNK(st->st_mode) ? 7 : 2;
    W64(buf + 24, st->st_nlink); W64(buf + 32, st->st_size);
    W64(buf + 40, (u64)st->st_atim.tv_sec * 1000000000ull + st->st_atim.tv_nsec);
    W64(buf + 48, (u64)st->st_mtim.tv_sec * 1000000000ull + st->st_mtim.tv_nsec);
    W64(buf + 56, (u64)st->st_ctim.tv_sec * 1000000000ull + st->st_ctim.tv_nsec);
}
WASI(fd_filestat_get), u32 fd, u32 buf) { int h = hfd(fd); struct stat st; if (h < 0 || fstat(h, &st)) return 8; put_filestat(buf, &st); return 0; }
WASI(path_filestat_get), u32 fd, u32 flags, u32 p, u32 len, u32 buf) { int h = hfd(fd); struct stat st; if (h < 0) return 8;
    if (fstatat(h, path_of(p, len), &st, (flags & 1) ? 0 : AT_SYMLINK_NOFOLLOW)) return werr(errno); put_filestat(buf, &st); return 0; }
WASI(path_create_directory), u32 fd, u32 p, u32 len) { int h = hfd(fd); if (h < 0) return 8;
    return mkdirat(h, path_of(p, len), 0777) ? werr(errno) : 0; }
WASI(path_unlink_file), u32 fd, u32 p, u32 len) { int h = hfd(fd); if (h < 0) return 8; return unlinkat(h, path_of(p, len), 0) ? werr(errno) : 0; }
WASI(path_remove_directory), u32 fd, u32 p, u32 len) { int h = hfd(fd); if (h < 0) return 8; return unlinkat(h, path_of(p, len), AT_REMOVEDIR) ? werr(errno) : 0; }
WASI(path_rename), u32 fd, u32 p, u32 len, u32 fd2, u32 p2, u32 len2) { int h = hfd(fd), h2 = hfd(fd2); if (h < 0 || h2 < 0) return 8;
    return renameat(h, path_of(p, len), h2, path_of(p2, len2)) ? werr(errno) : 0; }
WASI(path_open), u32 fd, u32 dirflags, u32 p, u32 len, u32 oflags, u64 rights, u64 rinh, u32 fdflags, u32 out) {
    (void)dirflags; (void)rinh;
    int h = hfd(fd); if (h < 0) return 8;
    int rd = (rights & ((1 << 1) | (1 << 14))) != 0, wr = (rights & ((1 << 6) | (1 << 0) | (1 << 3) | (1 << 22))) != 0;  /* FD_READ|READDIR, FD_WRITE|DATASYNC|ALLOCATE|FILESTAT_SET_SIZE */
    int fl = (rd && wr) ? O_RDWR : wr ? O_WRONLY : O_RDONLY;
    if (oflags & 1) fl |= O_CREAT; if (oflags & 2) fl |= O_DIRECTORY; if (oflags & 4) fl |= O_EXCL; if (oflags & 8) fl |= O_TRUNC;
    if (fdflags & 1) fl |= O_APPEND;
    int nh = openat(h, path_of(p, len), fl | O_CLOEXEC, 0666);
    if (nh < 0 && errno == EISDIR && fl != O_RDONLY) nh = openat(h, path_of(p, len), O_RDONLY | O_DIRECTORY | O_CLOEXEC);
    if (nh < 0) return werr(errno);
    for (u32 i = 4; i < 256; i++) if (fdmap[i] < 0) { fdmap[i] = nh; W32(out, i); return 0; }
    close(nh); return 76;   /* ENFILE */
}
WASI(poll_oneoff), u32 in, u32 outp, u32 n, u32 nev) {
    u32 k = 0;
    for (u32 i = 0; i < n; i++) {
        u32 s = in + 48 * i, e = outp + 32 * k;
        if (P(s)[8] == 0) {              /* clock */
            u32 id = R32(s + 16); u64 t; memcpy(&t, P(s + 24), 8); u16 fl; memcpy(&fl, P(s + 40), 2);
            struct timespec ts = { (time_t)(t / 1000000000ull), (long)(t % 1000000000ull) };
            clock_nanosleep(id == 0 ? CLOCK_REALTIME : CLOCK_MONOTONIC, (fl & 1) ? TIMER_ABSTIME : 0, &ts, 0);
        }
        memset(P(e), 0, 32); memcpy(P(e), P(s), 8); P(e)[10] = P(s)[8]; k++;
    }
    W32(nev, k); return 0;
}
WASI(sched_yield)) { return 0; }
WASI(random_get), u32 buf, u32 len) { for (u32 i = 0; i < len; i++) P(buf)[i] = (u8)rand(); return 0; }
void w2c_wasi__snapshot__preview1_proc_exit(struct w2c_wasi__snapshot__preview1 *u_, u32 code) { (void)u_; fflush(stdout); _exit((int)code); }

/* wasm-rt-impl.c is built with -DWASM_RT_TRAP_HANDLER=port_w2c_trap: a trap (out-of-bounds access, unreachable,
   a null table entry) is fatal and reported from whichever coroutine hit it */
void port_w2c_trap(wasm_rt_trap_t t) {
    fprintf(stderr, "[wasm] trap: %s (coroutine context %d)\n", wasm_rt_strerror(t), cur_ctx); fflush(stderr); _exit(4);
}

int main(int argc, char **argv) {
    g_argc = argc; g_argv = argv;
    char cwd[4096]; if (!getcwd(cwd, sizeof cwd)) strcpy(cwd, "/");
    snprintf(pwd_env, sizeof pwd_env, "PWD=%s", cwd);
    for (int i = 0; i < 256; i++) fdmap[i] = -1;
    fdmap[0] = 0; fdmap[1] = 1; fdmap[2] = 2; fdmap[3] = open("/", O_RDONLY | O_DIRECTORY);
    wasm_rt_init();
    wasm2c_tombi_instantiate(&inst, 0, 0);
    port_w2c_low_base = *w2c_tombi_port_ram(&inst);
    w2c_tombi_0x5Fstart(&inst);
    return 0;
}
