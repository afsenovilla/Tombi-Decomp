/* libmain.so: the SDL_main of the Android build (docs/PORT.md "4. Android").
 *
 * The game (libtombi.so) keeps the PC port's phase 1 pointer model: 32-bit pointers, and every game datum below
 * 16 MB so the 24-bit ordering-table links are exact (port_prelude.h, port_check_layout).  A shared library is
 * normally mapped wherever the dynamic linker likes, so this loader
 *   1. reserves a low range (PORT2_LOW_BASE, or a fallback base, sized from libtombi.so's own program headers) with
 *      mmap as early as possible (JNI_OnLoad, when SDLActivity loads libmain.so, before SDL's threads and the GL
 *      driver map anything there);
 *   2. in SDL_main, loads libtombi.so into that reservation with android_dlopen_ext(ANDROID_DLEXT_RESERVED_ADDRESS);
 *   3. needs the game data the launcher (LauncherActivity) extracted from the user's own disc image on the device
 *      (--data, the port's data/ mode; --disc from older versions); no game data is in the APK and there is no other
 *      source.  Without either a message box says to pick the disc;
 *   4. calls port_main with PC-style flags: the launcher's (--disc ...), --save-dir, --fullscreen, --renderer soft (the
 *      GLES path of shims/gpu_gl.c is untested here), then the words of files/args.txt (any PC flag).
 *
 * Self-diagnosis without adb: everything goes to tombi.log in the app's INTERNAL files dir (TombiActivity rotates it
 * and passes the dir in $TOMBI_LOG_DIR; the launcher shows it, copies / shares / exports it).  The log is opened first
 * thing in JNI_OnLoad; every loader step is written and fsync'd; stdout/stderr (the port's own log) are dup2'd into it;
 * a fatal-signal handler writes the signal, fault address, registers and a backtrace, then chains to the previous
 * handler (debuggerd); a watchdog thread reports the main thread when no frame comes for 10 s.  Markers in the same
 * dir: "running" (launcher, before the start; TombiActivity removes it on a clean finish) and "crashed" (written here
 * on a fatal signal, an abnormal exit or a start-up failure). */
#include <android/dlext.h>
#include <android/log.h>
#include <dlfcn.h>
#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <jni.h>
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/system_properties.h>
#include <dirent.h>
#include <sys/utsname.h>
#include <time.h>
#include <ucontext.h>
#include <unistd.h>
#include <unwind.h>
#include <SDL2/SDL.h>

#if defined(__LP64__)
/* arm64-v8a: libtombi.so is the wasm2c build (port2/wasm, docs/PORT.md "64-bit builds: wasm2c"): the game's 32-bit
   address space is the module's linear memory, so no low reservation is needed; the runtime variables the loader
   reads live there too (port_wasm_var_ptr, valid once port_main has instantiated the module) */
#include "../wasm/bridge_vars.h"
typedef Elf64_Ehdr ElfEhdr; typedef Elf64_Shdr ElfShdr; typedef Elf64_Sym ElfSym; typedef Elf64_Phdr ElfPhdr;
#define PORT_ELF_CLASS ELFCLASS64
#else
typedef Elf32_Ehdr ElfEhdr; typedef Elf32_Shdr ElfShdr; typedef Elf32_Sym ElfSym; typedef Elf32_Phdr ElfPhdr;
#define PORT_ELF_CLASS ELFCLASS32
#endif
#define PORT2_LOW_BASE 0x00100000ul
#define PORT2_LOW_SIZE 0x02000000ul      /* 32 MB at least: libtombi.so is ~19 MB with its .bss; port_ram must end below 16 MB */
#define TAG "tombi"

/* ------------------------------------------------------------------------------------------------ the log */
static int log_fd = -1;
static char log_dir[512];
static struct timespec t0;

static void raw_write(int fd, const char *s, size_t n) {
    while (n > 0) { long w = syscall(SYS_write, fd, s, n); if (w <= 0) break; s += w; n -= (size_t)w; }
}
static double since_start(void) {
    struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t);
    return (double)(t.tv_sec - t0.tv_sec) + (t.tv_nsec - t0.tv_nsec) / 1e9;
}
/* one "[loader +S.mmm] ..." line, written at once and fsync'd, mirrored to logcat */
static void tlog(const char *fmt, ...) {
    char b[1536]; int n = snprintf(b, sizeof b, "[loader +%.3f] ", since_start());
    va_list ap; va_start(ap, fmt);
    int m = vsnprintf(b + n, sizeof b - (size_t)n - 1, fmt, ap);
    va_end(ap);
    if (m < 0) m = 0;
    n += m; if (n > (int)sizeof b - 2) n = (int)sizeof b - 2;
    if (b[n - 1] != '\n') b[n++] = '\n';
    b[n] = 0;
    __android_log_write(ANDROID_LOG_INFO, TAG, b);
    if (log_fd >= 0) { raw_write(log_fd, b, (size_t)n); fsync(log_fd); }
}
static void marker(const char *name, const char *text) {
    char p[600]; snprintf(p, sizeof p, "%s/%s", log_dir, name);
    int fd = open(p, O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0644);
    if (fd >= 0) { raw_write(fd, text, strlen(text)); fsync(fd); close(fd); }
}
static void prop(const char *k, char *v) { v[0] = 0; __system_property_get(k, v); if (!v[0]) strcpy(v, "?"); }

/* the log dir: $TOMBI_LOG_DIR (TombiActivity: getFilesDir()), else /data/data/<package>/files from /proc/self/cmdline */
static void open_log(void) {
    if (log_fd >= 0) return;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    const char *env = getenv("TOMBI_LOG_DIR");
    int rotated_by_java = env && *env;
    if (rotated_by_java) snprintf(log_dir, sizeof log_dir, "%s", env);
    else {
        char cmd[256] = {0}; int fd = open("/proc/self/cmdline", O_RDONLY | O_CLOEXEC);
        if (fd >= 0) { long r = read(fd, cmd, sizeof cmd - 1); close(fd); if (r < 0) cmd[0] = 0; }
        char *colon = strchr(cmd, ':'); if (colon) *colon = 0;
        snprintf(log_dir, sizeof log_dir, "/data/data/%s/files", cmd[0] ? cmd : "io.github.tombi.port");
        mkdir(log_dir, 0700);
    }
    char p[600]; snprintf(p, sizeof p, "%s/tombi.log", log_dir);
    if (!rotated_by_java) { char q[600]; snprintf(q, sizeof q, "%s/tombi.prev.log", log_dir); rename(p, q); }
    log_fd = open(p, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC | (rotated_by_java ? 0 : O_TRUNC), 0644);
    if (log_fd < 0) { __android_log_print(ANDROID_LOG_ERROR, TAG, "cannot open %s: %s", p, strerror(errno)); return; }
    /* the port's own log (stderr, unbuffered: runtime/main.c) and anything on stdout land in the same file */
    if (dup2(log_fd, 2) < 0 || dup2(log_fd, 1) < 0) tlog("dup2 to stdout/stderr failed: %s", strerror(errno));
    char rel[PROP_VALUE_MAX], sdk[PROP_VALUE_MAX], man[PROP_VALUE_MAX], mod[PROP_VALUE_MAX], soc[PROP_VALUE_MAX],
         abi[PROP_VALUE_MAX], abi32[PROP_VALUE_MAX], fp[PROP_VALUE_MAX];
    prop("ro.build.version.release", rel); prop("ro.build.version.sdk", sdk); prop("ro.product.manufacturer", man);
    prop("ro.product.model", mod); prop("ro.soc.model", soc); prop("ro.product.cpu.abilist", abi);
    prop("ro.product.cpu.abilist32", abi32); prop("ro.build.fingerprint", fp);
    struct utsname u; memset(&u, 0, sizeof u); uname(&u);
    char mma[32] = "?"; { int fd = open("/proc/sys/vm/mmap_min_addr", O_RDONLY | O_CLOEXEC); if (fd >= 0) { long r = read(fd, mma, sizeof mma - 1); close(fd); mma[r > 0 ? r : 0] = 0; char *nl = strchr(mma, '\n'); if (nl) *nl = 0; } }
    tlog("libmain.so loaded (JNI_OnLoad): pid %d uid %d, log %s", getpid(), getuid(), p);
    tlog("device: %s %s (soc %s), Android %s (SDK %s), kernel %s %s, ABIs %s / 32-bit %s", man, mod, soc, rel, sdk, u.release, u.machine, abi, abi32);
    tlog("build: %s; page size %ld, mmap_min_addr %s", fp, sysconf(_SC_PAGESIZE), mma);
}

/* /proc/self/maps lines that overlap [lo, hi): what holds the range we want */
static void log_maps(unsigned long lo, unsigned long hi) {
    FILE *f = fopen("/proc/self/maps", "re");
    if (!f) { tlog("  (cannot read /proc/self/maps: %s)", strerror(errno)); return; }
    char line[512]; int n = 0;
    while (fgets(line, sizeof line, f)) {
        unsigned long a, b;
        if (sscanf(line, "%lx-%lx", &a, &b) != 2 || b <= lo || a >= hi) continue;
        char *nl = strchr(line, '\n'); if (nl) *nl = 0;
        tlog("  map: %s", line);
        if (++n >= 24) { tlog("  map: ..."); break; }
    }
    if (!n) tlog("  (nothing mapped in %#lx-%#lx)", lo, hi);
    fclose(f);
}

/* ------------------------------------------------------------------------------------------------ fatal signals */
static const int fatal_sigs[] = { SIGSEGV, SIGBUS, SIGILL, SIGABRT, SIGFPE, SIGTRAP, SIGSYS };
static struct sigaction old_act[65];
static void *tombi_h;                                        /* libtombi.so, once loaded */
static char game_lib[64] = "libtombi.so";                    /* the game library SDL_main loads (--game-lib) */
static const char *(*sym_near)(void *, unsigned long *);     /* libtombi's port_sym_near: names of game functions */
static int *frame_p;                                         /* libtombi's port_frame */
/* an intentional end (the game's "back to the launcher", the Back dialog, the activity finishing): libtombi's
   port_clean_exit or TombiActivity.nativeCleanExit; after it a fault or exit status while the process tears down is
   logged but not written as the "crashed" marker (the launcher's crash card is for abnormal ends only) */
static int *clean_exit_p; static volatile int clean_exit;
JNIEXPORT void JNICALL Java_io_github_tombi_port_TombiActivity_nativeCleanExit(JNIEnv *env, jclass c) { (void)env; (void)c; clean_exit = 1; }
static int *exit_status_p;                                   /* libtombi's port_exit_status (port_fatal / port_quit) */
#if defined(__LP64__)
static int *(*wasm_var)(int);                                /* libtombi's port_wasm_var_ptr (arm64: port2/wasm/host_sdl.c) */
static int *wvar(int *p, int id) { return wasm_var ? wasm_var(id) : p; }
#define frame_p wvar(frame_p, PORT_WASM_VAR_port_frame)
#define exit_status_p wvar(exit_status_p, PORT_WASM_VAR_port_exit_status)
#define clean_exit_p wvar(clean_exit_p, PORT_WASM_VAR_port_clean_exit)
#endif
static int intentional(void) { return clean_exit || (clean_exit_p && *clean_exit_p); }
static pthread_t main_thread; static int main_thread_set;
static volatile int in_crash;

static const char *signame(int s) {
    switch (s) { case SIGSEGV: return "SIGSEGV"; case SIGBUS: return "SIGBUS"; case SIGILL: return "SIGILL";
                 case SIGABRT: return "SIGABRT"; case SIGFPE: return "SIGFPE"; case SIGTRAP: return "SIGTRAP";
                 case SIGSYS: return "SIGSYS"; case SIGUSR2: return "SIGUSR2"; default: return "?"; }
}
/* libtombi.so's full symbol table (.symtab: static functions too; the APK keeps it, app/build.gradle
   keepDebugSymbols, with the DWARF already removed by port2/CMakeLists.txt).  Mapped once after the load, read by
   the crash handler: dladdr only knows the exported names, and port_sym_near only the game functions, so a fault in
   the runtime was named after the nearest game function ("FUN_8005aa74+0x9098" for port_printf) */
static const ElfSym *st_sym; static unsigned st_n; static const char *st_str; static unsigned st_strsz;
static unsigned long st_base;
static void symtab_load(const char *path, unsigned long base) {
    int fd = open(path, O_RDONLY | O_CLOEXEC); struct stat sb;
    if (fd < 0 || fstat(fd, &sb) || sb.st_size < (off_t)sizeof(ElfEhdr)) { if (fd >= 0) close(fd); return; }
    const unsigned char *f = mmap(0, (size_t)sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0); close(fd);
    if (f == MAP_FAILED) return;
    size_t sz = (size_t)sb.st_size; const ElfEhdr *eh = (const ElfEhdr *)f;
    if (memcmp(eh->e_ident, ELFMAG, SELFMAG) || eh->e_ident[EI_CLASS] != PORT_ELF_CLASS || eh->e_shentsize != sizeof(ElfShdr)
        || eh->e_shoff + (size_t)eh->e_shnum * sizeof(ElfShdr) > sz) { munmap((void *)f, sz); return; }
    const ElfShdr *sh = (const ElfShdr *)(f + eh->e_shoff);
    for (unsigned i = 0; i < eh->e_shnum; i++) {
        if (sh[i].sh_type != SHT_SYMTAB || sh[i].sh_link >= eh->e_shnum) continue;
        const ElfShdr *ss = &sh[sh[i].sh_link];
        if (sh[i].sh_offset + sh[i].sh_size > sz || ss->sh_offset + ss->sh_size > sz) break;
        st_sym = (const ElfSym *)(f + sh[i].sh_offset); st_n = sh[i].sh_size / sizeof(ElfSym);
        st_str = (const char *)(f + ss->sh_offset); st_strsz = ss->sh_size; st_base = base;
        tlog("libtombi.so symbol table: %u symbols", st_n);
        return;
    }
    tlog("libtombi.so has no symbol table (stripped): crash addresses are named after exported symbols only");
    munmap((void *)f, sz);
}
static const char *symtab_near(unsigned long a, unsigned long *off) {
    if (!st_sym || a < st_base) return 0;
    unsigned long v = a - st_base, best = 0; const char *nm = 0;
    for (unsigned i = 1; i < st_n; i++) {
        const ElfSym *s = &st_sym[i];
        if (((s->st_info) & 0xf) != STT_FUNC || s->st_shndx == SHN_UNDEF || s->st_name >= st_strsz) continue;
        unsigned long sv = s->st_value & ~1ul;
        if (sv <= v && (!nm || sv > best)) { best = sv; nm = st_str + s->st_name; }
    }
    if (nm && off) *off = v - best;
    return nm;
}
/* "0x1234 libtombi.so+0x5678 (sym+0x9)": the nearest of the candidate names (full symbol table, the game's own
   function map, dladdr's exported symbol) */
static void describe(unsigned long a, char *o, size_t n) {
    Dl_info di; memset(&di, 0, sizeof di);
    if (a && dladdr((void *)a, &di) && di.dli_fname) {
        const char *base = strrchr(di.dli_fname, '/'); base = base ? base + 1 : di.dli_fname;
        unsigned long off = 0, o2 = 0; const char *nm = 0, *n2;
        if (sym_near && strstr(base, "libtombi")) nm = sym_near((void *)a, &off);
        if (nm && !strcmp(nm, "?")) nm = 0;
        if (strstr(base, "libtombi") && (n2 = symtab_near(a, &o2)) && (!nm || o2 < off)) { nm = n2; off = o2; }
        if (di.dli_sname && (!nm || a - (unsigned long)di.dli_saddr < off)) { nm = di.dli_sname; off = a - (unsigned long)di.dli_saddr; }
        if (nm) snprintf(o, n, "%08lx %s+0x%lx (%s+0x%lx)", a, base, a - (unsigned long)di.dli_fbase, nm, off);
        else snprintf(o, n, "%08lx %s+0x%lx", a, base, a - (unsigned long)di.dli_fbase);
    } else snprintf(o, n, "%08lx ?", a);
}
struct bt { int n; unsigned long last; };
static _Unwind_Reason_Code bt_step(struct _Unwind_Context *c, void *arg) {
    struct bt *b = arg; unsigned long ip = _Unwind_GetIP(c);
    if (!ip || b->n >= 32 || (b->n && ip == b->last)) return _URC_END_OF_STACK;
    char d[300]; describe(ip, d, sizeof d);
    tlog("  #%02d %s", b->n++, d); b->last = ip;
    return _URC_NO_REASON;
}
/* registers, the unwinder's backtrace and the code addresses found on the stack */
static void dump_context(ucontext_t *uc) {
    char d[300];
#if defined(__arm__)
    mcontext_t *m = &uc->uc_mcontext;
    describe(m->arm_pc, d, sizeof d); tlog("  pc %s", d);
    describe(m->arm_lr, d, sizeof d); tlog("  lr %s", d);
    tlog("  r0 %08lx r1 %08lx r2 %08lx r3 %08lx r4 %08lx r5 %08lx r6 %08lx r7 %08lx", m->arm_r0, m->arm_r1, m->arm_r2, m->arm_r3, m->arm_r4, m->arm_r5, m->arm_r6, m->arm_r7);
    tlog("  r8 %08lx r9 %08lx r10 %08lx fp %08lx ip %08lx sp %08lx cpsr %08lx", m->arm_r8, m->arm_r9, m->arm_r10, m->arm_fp, m->arm_ip, m->arm_sp, m->arm_cpsr);
    unsigned long sp = m->arm_sp;
#elif defined(__aarch64__)
    describe(uc->uc_mcontext.pc, d, sizeof d); tlog("  pc %s", d);
    describe(uc->uc_mcontext.regs[30], d, sizeof d); tlog("  lr %s", d);
    unsigned long sp = uc->uc_mcontext.sp;
#else
    unsigned long sp = 0; (void)uc;
#endif
    tlog("  backtrace (unwinder):");
    struct bt b = { 0, 0 }; _Unwind_Backtrace(bt_step, &b);
    if (!sp) return;
    tlog("  code addresses on the stack (sp %08lx):", sp);
    int found = 0;
    for (int i = 0; i < 1024 && found < 24; i++) {
        unsigned long v = ((unsigned long *)sp)[i];
        Dl_info di;
        if (v < 0x1000 || !dladdr((void *)v, &di) || !di.dli_fname) continue;
        if (!strstr(di.dli_fname, ".so") && !strstr(di.dli_fname, "app_process")) continue;
        describe(v, d, sizeof d); tlog("  sp+%03x %s", i * 4, d); found++;
    }
}
static void on_fatal(int sig, siginfo_t *info, void *ucv) {
    int first = !in_crash; in_crash = 1;
    if (first) {
        char tn[32] = "?"; { int fd = open("/proc/thread-self/comm", O_RDONLY | O_CLOEXEC); if (fd >= 0) { long r = read(fd, tn, sizeof tn - 1); close(fd); tn[r > 0 ? r : 0] = 0; char *nl = strchr(tn, '\n'); if (nl) *nl = 0; } }
        tlog("*** FATAL SIGNAL %d (%s), code %d, fault address %p, thread %ld (%s)%s, frame %d", sig, signame(sig), info->si_code,
             info->si_addr, (long)syscall(SYS_gettid), tn, main_thread_set && pthread_equal(pthread_self(), main_thread) ? " = game thread" : "",
             frame_p ? *frame_p : -1);
        dump_context((ucontext_t *)ucv);
        char m[96]; snprintf(m, sizeof m, "signal %d (%s) at frame %d\n", sig, signame(sig), frame_p ? *frame_p : -1);
        if (intentional()) tlog("(after an intentional quit: not marked as a crash)"); else marker("crashed", m);
        tlog("*** end of crash report: handing the signal on (debuggerd / default)");
    }
    /* chain: the previous handler (debuggerd's tombstone, or the default action) */
    sigaction(sig, &old_act[sig], NULL);
    if (info->si_code <= 0 || sig == SIGABRT) syscall(SYS_tgkill, getpid(), syscall(SYS_gettid), sig);   /* sent, not a fault */
    /* a fault re-executes the instruction on return and reaches the previous handler */
}
static unsigned char alt_stack_main[65536];
static void install_fatal(void) {
    stack_t ss; memset(&ss, 0, sizeof ss);
    ss.ss_sp = alt_stack_main; ss.ss_size = sizeof alt_stack_main;
    sigaltstack(&ss, 0);
    struct sigaction sa; memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = on_fatal; sa.sa_flags = SA_SIGINFO | SA_ONSTACK;
    sigemptyset(&sa.sa_mask);
    for (unsigned i = 0; i < sizeof fatal_sigs / sizeof fatal_sigs[0]; i++) sigaction(fatal_sigs[i], &sa, &old_act[fatal_sigs[i]]);
}

/* ------------------------------------------------------------------------------------------------ watchdog */
static void on_usr2(int sig, siginfo_t *info, void *ucv) {
    (void)sig; (void)info;
    tlog("watchdog: game thread state:");
    dump_context((ucontext_t *)ucv);
}
static volatile int wd_phase;   /* 0 loading, 1 in port_main, 2 a start-up message box is up (no reports) */
static void *wd_main(void *arg) {
    (void)arg;
    int last = -1, still = 0, reports = 0; static const int marks[] = { 1, 2, 10, 60, 300, 1500 }; unsigned mk = 0;
    for (;;) {
        sleep(1);
        int f = frame_p ? *frame_p : 0;
        while (mk < sizeof marks / sizeof marks[0] && f >= marks[mk]) { tlog("frame %d reached", marks[mk]); mk++; }
        if (f != last) { last = f; still = 0; continue; }
        if (wd_phase == 2) { still = 0; continue; }   /* waiting for the user on a message box: not a hang */
        if (++still % 10 || reports >= 3) continue;
        reports++;
        tlog("watchdog: no new frame for %d s (frame %d, %s)", still, f, wd_phase ? "in port_main" : "loading");
        if (main_thread_set) pthread_kill(main_thread, SIGUSR2);
    }
    return 0;
}
static void start_watchdog(void) {
    struct sigaction sa; memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = on_usr2; sa.sa_flags = SA_SIGINFO | SA_RESTART | SA_ONSTACK;
    sigaction(SIGUSR2, &sa, 0);
    pthread_t t;
    if (pthread_create(&t, 0, wd_main, 0) == 0) pthread_detach(t); else tlog("watchdog: pthread_create failed");
}

/* exit() from inside the game (port_fatal -> exit(1), port_quit(code)): a non-zero status is a crash for the launcher */
static void at_exit(void) {
    int st = exit_status_p ? *exit_status_p : -1;
    tlog("process exit() (game status %d)", st);
    if (st > 0 && !intentional()) { char m[64]; snprintf(m, sizeof m, "exit status %d\n", st); marker("crashed", m); }
}

/* ------------------------------------------------------------------------------------------------ the low range */
static void *low_base;
static unsigned long low_size = PORT2_LOW_SIZE;
static char low_err[256];
static char lib_dir[512];

/* the span one game library needs (its PT_LOAD segments, .bss included), read from the file */
static unsigned long lib_span(const char *p) {
    int fd = open(p, O_RDONLY | O_CLOEXEC);
    if (fd < 0) { tlog("cannot open %s: %s (the APK's native libraries were not extracted?)", p, strerror(errno)); return 0; }
    ElfEhdr eh; unsigned long hi = 0; struct stat st; fstat(fd, &st);
    if (pread(fd, &eh, sizeof eh, 0) == sizeof eh && !memcmp(eh.e_ident, ELFMAG, SELFMAG) && eh.e_ident[EI_CLASS] == PORT_ELF_CLASS) {
        for (int i = 0; i < eh.e_phnum; i++) {
            ElfPhdr ph;
            if (pread(fd, &ph, sizeof ph, eh.e_phoff + (off_t)i * eh.e_phentsize) != sizeof ph) break;
            if (ph.p_type == PT_LOAD && ph.p_vaddr + ph.p_memsz > hi) hi = ph.p_vaddr + ph.p_memsz;
        }
    } else tlog("%s is not a 32-bit ELF", p);
    close(fd);
    tlog("%s: %ld bytes on disk, load span %#lx (%.1f MB)", p, (long)st.st_size, hi, hi / 1048576.0);
    return hi;
}
/* the span the reservation needs: the largest of the game libraries next to libmain.so (libtombi.so for PAL,
   libtombi_usa.so ...: one per game version, LauncherActivity GameVersion), as JNI_OnLoad runs before the launcher's
   arguments say which one SDL_main will load */
static unsigned long tombi_span(void) {
    Dl_info di;
    if (!dladdr((void *)tombi_span, &di) || !di.dli_fname) { tlog("dladdr(libmain.so) failed: library dir unknown"); return 0; }
    snprintf(lib_dir, sizeof lib_dir, "%s", di.dli_fname);
    char *sl = strrchr(lib_dir, '/'); if (sl) *sl = 0;
    unsigned long hi = 0; int n = 0;
    DIR *d = opendir(lib_dir);
    if (d) {
        struct dirent *e;
        while ((e = readdir(d))) {
            size_t l = strlen(e->d_name);
            if (strncmp(e->d_name, "libtombi", 8) || l < 4 || strcmp(e->d_name + l - 3, ".so") || !strcmp(e->d_name, "libtombidisc.so")) continue;
            char p[800]; snprintf(p, sizeof p, "%s/%s", lib_dir, e->d_name);
            unsigned long h = lib_span(p); n++;
            if (h > hi) hi = h;
        }
        closedir(d);
    }
    if (!n) { char p[600]; snprintf(p, sizeof p, "%s/libtombi.so", lib_dir); hi = lib_span(p); }
    return hi;
}

static int try_reserve(unsigned long base, unsigned long size) {
    void *p = mmap((void *)base, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
    if (p == MAP_FAILED) {
        snprintf(low_err, sizeof low_err, "mmap(%#lx, %#lx): %s", base, size, strerror(errno));
        tlog("reserve %#lx +%#lx: %s", base, size, low_err);
        return 0;
    }
    if (p != (void *)base) {      /* only a hint without MAP_FIXED (which would clobber a mapping there) */
        snprintf(low_err, sizeof low_err, "the range at %#lx is in use (mmap gave %p)", base, p);
        tlog("reserve %#lx +%#lx: in use (mmap gave %p); what is there:", base, size, p);
        munmap(p, size);
        log_maps(base, base + size);
        return 0;
    }
    low_base = p; low_size = size;
    tlog("reserved %p-%#lx (%lu MB)", p, base + size, size >> 20);
    return 1;
}
/* lowest page-aligned base >= 64 KB where [base, base+size) touches no mapping and base stays below 16 MB (port_ram
   has to end there; port_check_layout in libtombi.so checks the exact layout); 0 when there is none */
static unsigned long find_low_gap(unsigned long size) {
    FILE *f = fopen("/proc/self/maps", "re");
    if (!f) return 0;
    unsigned long cand = 0x00010000ul, found = 0; char line[512];
    while (fgets(line, sizeof line, f)) {
        unsigned long a, b;
        if (sscanf(line, "%lx-%lx", &a, &b) != 2) continue;
        if (b <= cand) continue;
        if (a >= cand + size) break;                 /* the gap before this mapping fits */
        cand = (b + 0xFFFFul) & ~0xFFFFul;           /* past it */
        if (cand >= 0x01000000ul) break;
    }
    fclose(f);
    if (cand < 0x01000000ul) found = cand;
    tlog("free low gap for %#lx: %#lx", size, found);
    return found;
}
/* what holds the range: a libtombi.so / libmain.so of ours (a previous game instance in a reused process) or something else */
static int low_stale_self;
static void low_conflict_describe(unsigned long size) {
    FILE *f = fopen("/proc/self/maps", "re");
    if (!f) return;
    char line[512], who[160] = "";
    while (fgets(line, sizeof line, f)) {
        unsigned long a, b;
        if (sscanf(line, "%lx-%lx", &a, &b) != 2 || b <= PORT2_LOW_BASE || a >= PORT2_LOW_BASE + size) continue;
        if (strstr(line, "libtombi.so")) low_stale_self = 1;
        if (!who[0]) { char *nl = strchr(line, '\n'); if (nl) *nl = 0; snprintf(who, sizeof who, "%s", line); }
    }
    fclose(f);
    if (who[0]) {
        size_t l = strlen(low_err);
        snprintf(low_err + l, sizeof low_err - l, "; first mapping in the way: %.120s", who);
    }
    tlog("low range conflict: %s%s", low_err, low_stale_self ? " (a previous game instance of this process)" : "");
}
static void reserve_low(void) {
    if (low_base) return;
    unsigned long span = tombi_span();
    unsigned long size = PORT2_LOW_SIZE;
    if (span + 0x100000ul > size) {
        size = (span + 0x100000ul + 0xFFFFFul) & ~0xFFFFFul;
        tlog("libtombi.so needs more than %lu MB: reserving %lu MB", PORT2_LOW_SIZE >> 20, size >> 20);
    }
    /* the usual base first; then other low bases (port_check_layout in libtombi.so says whether the data image still
       ends below 16 MB there) */
    static const unsigned long bases[] = { PORT2_LOW_BASE, 0x00010000ul, 0x00200000ul, 0x00400000ul, 0x00800000ul };
    for (unsigned i = 0; i < sizeof bases / sizeof bases[0]; i++) if (try_reserve(bases[i], size)) return;
    /* every usual base is taken: the lowest free gap below 16 MB + size in /proc/self/maps (whatever sits in the way) */
    unsigned long gap = find_low_gap(size);
    if (gap && try_reserve(gap, size)) return;
    low_conflict_describe(size);
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    (void)vm; (void)reserved;
    open_log();
    install_fatal();
#if !defined(__LP64__)
    reserve_low();
#endif
    if (!low_base && sizeof(void *) == 4) tlog("JNI_OnLoad: NO low range reserved (%s); SDL_main will retry", low_err);
    return JNI_VERSION_1_4;
}

/* ------------------------------------------------------------------------------------------------ SDL_main */
static int fail(const char *title, const char *msg) {
    tlog("FAILED: %s: %s", title, msg);
    char m[1200]; snprintf(m, sizeof m, "start-up failure: %s\n", msg);
    marker("crashed", m);
    wd_phase = 2;   /* the box waits for the user (a JNI call blocked in Java): the watchdog must not call it a hang */
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, msg, NULL);
    return 1;
}

int SDL_main(int argc, char *argv[]) {
    static char msg[1024], saves[512], argsbuf[2048], filesbuf[512];
    char *args[72]; int n = 0;
    open_log();                       /* (no-op: JNI_OnLoad opened it) */
    static int runs;
    if (runs++) {   /* the activity came back in a ":game" process that already ran the game: its state is used up */
        tlog("SDL_main called again in the same process (run %d): the game cannot restart here", runs);
        wd_phase = 2;
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Tombi!", "The game was already started in this process.\n\nClose Tombi! and start it again from the launcher.", NULL);
        exit(0);
    }
    main_thread = pthread_self(); main_thread_set = 1;
    { stack_t ss; memset(&ss, 0, sizeof ss); ss.ss_sp = alt_stack_main; ss.ss_size = sizeof alt_stack_main; sigaltstack(&ss, 0); }
    start_watchdog();
    atexit(at_exit);
    tlog("SDL_main: %d arguments from the launcher", argc - 1);
    for (int i = 1; argv && i < argc && argv[i]; i++) tlog("  argv[%d] = %s", i, argv[i]);
    { char m[64]; snprintf(m, sizeof m, "pid %d\n", getpid()); marker("running", m); }

    /* the game's files dir (disc copy, saves, args.txt): the launcher's (--files-dir), else SDL's external one */
    const char *files = NULL, *disc = NULL, *data = NULL, *save_sub = "saves";
    for (int i = 1; argv && i + 1 < argc && argv[i]; i++) {
        if (!strcmp(argv[i], "--files-dir")) files = argv[i + 1];
        if (!strcmp(argv[i], "--disc")) disc = argv[i + 1];
        if (!strcmp(argv[i], "--data")) data = argv[i + 1];
        /* the game version (LauncherActivity GameVersion): its library (libtombi.so = PAL, libtombi_usa.so ...) and
           its own saves folder under the files dir; plain names only */
        if (!strcmp(argv[i], "--game-lib") && !strncmp(argv[i + 1], "libtombi", 8) && !strchr(argv[i + 1], '/'))
            snprintf(game_lib, sizeof game_lib, "%s", argv[i + 1]);
        if (!strcmp(argv[i], "--save-sub") && argv[i + 1][0] && !strchr(argv[i + 1], '/') && argv[i + 1][0] != '.') save_sub = argv[i + 1];
    }
    if (!files) { files = SDL_AndroidGetExternalStoragePath(); tlog("no --files-dir: SDL external storage path %s", files ? files : "(null)"); }
    if (!files) { tlog("no external storage (%s): using the internal dir", SDL_GetError()); snprintf(filesbuf, sizeof filesbuf, "%s", log_dir); files = filesbuf; }
    snprintf(saves, sizeof saves, "%s/%s", files, save_sub);
    if (mkdir(files, 0755) != 0 && errno != EEXIST) tlog("mkdir %s: %s", files, strerror(errno));
    if (mkdir(saves, 0755) != 0 && errno != EEXIST) tlog("mkdir %s: %s", saves, strerror(errno));
    if (chdir(files) != 0) tlog("chdir %s: %s", files, strerror(errno));
    tlog("files dir %s, saves %s, game library %s", files, saves, game_lib);

    if (data) {   /* the launcher's extraction (LauncherActivity.importDisc, disccheck.c): data/manifest.txt */
        char mp[700]; snprintf(mp, sizeof mp, "%s/manifest.txt", data);
        struct stat st;
        if (stat(mp, &st) != 0 || access(mp, R_OK) != 0) {
            snprintf(msg, sizeof msg, "The extracted game data is missing or unreadable\n%s\n(%s).\n\nPick your disc again in the launcher (Disc).", mp, strerror(errno));
            return fail("Tombi!", msg);
        }
        tlog("data %s: manifest %lld bytes", data, (long long)st.st_size);
    } else if (!disc)
        return fail("Tombi! - pick your disc", "No disc image.\n\nOpen the Tombi! launcher and pick the image of your own disc "
                    "(Disc).\n\nThe app ships no game data.");
    if (!data) {   /* the disc image (and the .bin next to a .cue) must be readable from this process */
        struct stat st; int fd = open(disc, O_RDONLY | O_CLOEXEC);
        if (fd < 0 || fstat(fd, &st) != 0) {
            snprintf(msg, sizeof msg, "Cannot open the disc image\n%s\n(%s).\n\nPick your disc again in the launcher (Disc).", disc, strerror(errno));
            if (fd >= 0) close(fd);
            return fail("Tombi!", msg);
        }
        unsigned char head[16] = {0}; long r = pread(fd, head, sizeof head, 0); close(fd);
        tlog("disc %s: %lld bytes, readable (first bytes %02x %02x %02x %02x, read %ld)", disc, (long long)st.st_size, head[0], head[1], head[2], head[3], r);
    }

#if defined(__LP64__)
    tlog("dlopen(%s) (arm64: the wasm2c build, no low reservation)", game_lib);
    void *h = dlopen(game_lib, RTLD_NOW | RTLD_LOCAL);
    if (!h) {
        const char *e = dlerror();
        snprintf(msg, sizeof msg, "Loading %s failed: %s", game_lib, e ? e : "?");
        return fail("Tombi!", msg);
    }
#else
    reserve_low();
    if (!low_base && low_stale_self) {
        /* our own previous instance still holds it (Android reused the ":game" process): not a real conflict. No
           crash marker; the process ends so the next start from the launcher gets a fresh one */
        tlog("low range held by a previous game instance in this process: ending the process");
        marker("clean", "stale game process ended\n");
        wd_phase = 2;
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Tombi!", "The previous game was still closing.\n\nPlease start Tombi! again from the launcher.", NULL);
        _exit(0);
    }
    if (!low_base) {
        snprintf(msg, sizeof msg, "Another library of this app's process already uses the low memory addresses the game must be loaded at, so it cannot start on this device as it is (%s).\n\nRestarting the device may help. Please send the log (launcher: Options > View last log).", low_err);
        return fail("Tombi! cannot start", msg);
    }
    android_dlextinfo ext; memset(&ext, 0, sizeof ext);
    ext.flags = ANDROID_DLEXT_RESERVED_ADDRESS;
    ext.reserved_addr = low_base;
    ext.reserved_size = low_size;
    tlog("android_dlopen_ext(%s, RESERVED_ADDRESS %p +%#lx)", game_lib, low_base, low_size);
    void *h = android_dlopen_ext(game_lib, RTLD_NOW | RTLD_LOCAL, &ext);
    if (!h) {
        const char *e = dlerror();
        snprintf(msg, sizeof msg, "Loading %s failed: %s", game_lib, e ? e : "?");
        log_maps((unsigned long)low_base, (unsigned long)low_base + low_size);
        return fail("Tombi!", msg);
    }
#endif
    tombi_h = h;
    int (*port_main)(int, char **) = (int (*)(int, char **))dlsym(h, "port_main");
    sym_near = (const char *(*)(void *, unsigned long *))dlsym(h, "port_sym_near");
#if defined(__LP64__)
    wasm_var = (int *(*)(int))dlsym(h, "port_wasm_var_ptr");
    if (!wasm_var) tlog("libtombi.so has no port_wasm_var_ptr");
#else
    frame_p = (int *)dlsym(h, "port_frame");
    exit_status_p = (int *)dlsym(h, "port_exit_status");
    clean_exit_p = (int *)dlsym(h, "port_clean_exit");
#endif
    { Dl_info di; if (port_main && dladdr((void *)port_main, &di)) {
        tlog("libtombi.so loaded at %p (port_main %p); port_frame %p", di.dli_fbase, (void *)port_main, (void *)frame_p);
        if (di.dli_fname) symtab_load(di.dli_fname, (unsigned long)di.dli_fbase);
    } }
    if (!port_main) return fail("Tombi!", "libtombi.so has no port_main.");

    args[n++] = argv && argv[0] ? argv[0] : (char *)"tombi";
    args[n++] = (char *)"--save-dir"; args[n++] = saves;
    args[n++] = (char *)"--fullscreen";
    args[n++] = (char *)"--renderer"; args[n++] = (char *)"soft";
    args[n++] = (char *)"--widescreen"; args[n++] = (char *)"auto";   /* fit the panel (args.txt "--aspect 4:3" turns it off) */
    /* the launcher's choices (LauncherActivity -> TombiActivity.getArguments(): SDL passes them as argv[1..]),
       without the loader's own --files-dir (port_main would stop at an unknown flag) */
    for (int i = 1; argv && i < argc && argv[i] && n < 60; i++) {
        if (!strcmp(argv[i], "--files-dir") || !strcmp(argv[i], "--game-lib") || !strcmp(argv[i], "--save-sub")) { i++; continue; }
        args[n++] = argv[i];
    }
    {   /* files/args.txt: extra PC flags, whitespace separated (later flags override the defaults above) */
        char ap[600]; snprintf(ap, sizeof ap, "%s/args.txt", files);
        FILE *f = fopen(ap, "re");
        if (f) {
            size_t len = fread(argsbuf, 1, sizeof argsbuf - 1, f); fclose(f);
            argsbuf[len] = 0;
            for (char *t = strtok(argsbuf, " \t\r\n"); t && n < 71; t = strtok(0, " \t\r\n")) args[n++] = t;
            tlog("args.txt read (%u bytes)", (unsigned)len);
        }
    }
    args[n] = 0;
    {   char line[1400]; int o = 0;
        for (int i = 1; i < n && o < (int)sizeof line - 2; i++) o += snprintf(line + o, sizeof line - (size_t)o, " %s", args[i]);
        tlog("port_main(%d args):%s", n, line);
    }
    wd_phase = 1;
    int r = port_main(n, args);
    tlog("port_main returned %d", r);
    if (r != 0) { char m[64]; snprintf(m, sizeof m, "port_main returned %d\n", r); marker("crashed", m); }
    return r;
}
