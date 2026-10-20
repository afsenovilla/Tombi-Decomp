/* Watchdog: a host thread that, when the vblank counter has not advanced for 3 s, prints where the game thread is
 * (EIP with its symbol, ESP, the coroutine, the return addresses found on its stack) and the last shim calls
 * (port_trace ring), then lets it run on.  A silent hang on a user's machine then leaves a conclusive log.
 * Win32: SuspendThread + GetThreadContext on the game thread.  POSIX: SIGUSR1 to the game thread, whose
 * handler reads the ucontext.  --stall FRAME:MS (shims/psyq.c) is the test: a stall longer than 3 s fires it. */
#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <ucontext.h>
#endif
#include "port_runtime.h"

/* ---- the trace ring: the last shim calls, newest last */
#define TRACE_N 16
static struct { const char *what; int arg; int frame; } trace[TRACE_N];
static volatile unsigned trace_n;
void port_trace(const char *what, int arg) {
    unsigned i = trace_n % TRACE_N;
    trace[i].what = what; trace[i].arg = arg; trace[i].frame = port_frame;
    __sync_synchronize();
    trace_n++;
}
static void trace_print(void) {
    unsigned n = trace_n, from = n > TRACE_N ? n - TRACE_N : 0;
    port_printf("[port2] watchdog: last shim calls (oldest first):");
    for (unsigned k = from; k < n; k++) { unsigned i = k % TRACE_N; port_printf(" %s(%d)@%d", trace[i].what, trace[i].arg, trace[i].frame); }
    port_printf("\n");
}

static void report(unsigned long eip, unsigned long esp, unsigned long ebp) {
    unsigned long off; const char *nm = port_sym_near((void *)eip, &off);
    if (off > 0x100000ul) { nm = "outside the game code: libc/SDL/OS"; off = 0; }
    port_printf("[port2] watchdog: no vblank for 3 s at frame %d: eip=%08lx (%s+0x%lx) esp=%08lx ebp=%08lx thread %d; stack:",
                port_frame, eip, nm, off, esp, ebp, port_thread_current());
    port_dump_stack(esp);
    port_printf("\n");
    trace_print();
    port_low_report();
    port_flush_log();
}

static volatile int wd_fired;   /* reports: at most 3 per run, so a long stall does not flood the log */
int port_watchdog_seconds = 3;

/* ---- the second test: vblanks advance but the game does not.  Every task yields from the same call sites, the
   coroutines are switched, and every DrawOTag draws the same packets (the real-Windows freeze of build 810e31e:
   fps 47.6, 700 packets per frame for thousands of frames, no watchdog line because port_frame moved).  The
   signature is read from the host thread without stopping the game: it is the yield sites of the coroutines
   (threads.c) and the hash of the last ordering table (gpu.c). */
static unsigned progress_sig(void) {
    unsigned h = port_ot_signature;
    for (int n = 1; n <= 8; n++) {
        if (!port_thread_used(n)) continue;
        for (int k = 0; k < 4; k++) h = h * 1000003u ^ (unsigned)port_thread_yield_site[n][k] ^ (unsigned)n;
    }
    return h;
}
static void report_progress(int frames, unsigned switches) {
    port_printf("[port2] watchdog: no game progress for %d s (frame %d: %d vblanks ran, %u task switches, every task yields from the same place and every DrawOTag draws the same list, signature %08x); thread %d;",
                port_watchdog_seconds, port_frame, frames, switches, port_ot_signature, port_thread_current());
    for (int n = 1; n <= 8; n++) {
        if (!port_thread_used(n)) continue;
        port_printf(" task %d yields from", n);
        for (int k = 0; k < 4 && port_thread_yield_site[n][k]; k++) {
            unsigned long off; const char *nm = port_sym_near((void *)port_thread_yield_site[n][k], &off);
            port_printf(" %08lx(%s+0x%lx)", port_thread_yield_site[n][k], nm, off);
        }
        port_printf(";");
    }
    port_printf(" scratch: vbl %u/%u drawsync %u pause %04x mode %d; pad %04x (game reads %04x)\n",
                SPAD16(0x1E8), SPAD16(0x1EA), SPAD16(0x1EC), SPAD16(0x1F0), (signed char)SPAD8(0x1CC),
                port_pad_buttons[0], SPAD16(0x1FC));
    trace_print();
    port_flush_log();
}
/* once a second from the host thread: 1 = the vblank counter stalled (the caller stops the game and reports),
   0 = nothing to do (the "no progress" report, if due, was printed here) */
static int wd_tick(void) {
    static int last = -1, still = 0, pstill = 0, pframe = -1; static unsigned psig, pswitch;
    int f = port_frame;
    if (f == last) {
        pstill = 0;
        if (++still < port_watchdog_seconds || wd_fired >= 3) return 0;
        still = 0; wd_fired++;
        return 1;
    }
    last = f; still = 0;
    unsigned sig = progress_sig();
    if (sig == psig && pframe >= 0) {
        if (++pstill >= port_watchdog_seconds && wd_fired < 3) {
            wd_fired++; pstill = 0;
            report_progress(f - pframe, port_thread_switches - pswitch);
        }
    } else { pstill = 0; psig = sig; pframe = f; pswitch = port_thread_switches; }
    return 0;
}

#ifdef _WIN32
static HANDLE game_thread;
static DWORD WINAPI wd_main(void *arg) {
    (void)arg;
    for (;;) {
        Sleep(1000);
        if (!wd_tick()) continue;
        CONTEXT c; memset(&c, 0, sizeof c); c.ContextFlags = CONTEXT_CONTROL | CONTEXT_INTEGER;
        _lock_file(stderr);     /* never suspend the game thread inside its own fprintf: the report would wait for its lock */
        if (SuspendThread(game_thread) == (DWORD)-1) { port_printf("[port2] watchdog: SuspendThread failed (%lu)\n", GetLastError()); _unlock_file(stderr); continue; }
        if (GetThreadContext(game_thread, &c)) report(c.Eip, c.Esp, c.Ebp);
        else port_printf("[port2] watchdog: GetThreadContext failed (%lu)\n", GetLastError());
        ResumeThread(game_thread);
        _unlock_file(stderr);
    }
    return 0;
}
void port_watchdog_start(void) {
    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &game_thread, 0, FALSE, DUPLICATE_SAME_ACCESS);
    HANDLE h = CreateThread(0, 65536, wd_main, 0, 0, 0);
    if (h) CloseHandle(h); else port_printf("[port2] watchdog: CreateThread failed (%lu)\n", GetLastError());
}
#else
static pthread_t game_thread;
static void on_usr1(int sig, siginfo_t *info, void *ucv) {
    ucontext_t *uc = ucv; (void)sig; (void)info;
    report(uc->uc_mcontext.gregs[REG_EIP], uc->uc_mcontext.gregs[REG_ESP], uc->uc_mcontext.gregs[REG_EBP]);
}
static void *wd_main(void *arg) {
    (void)arg;
    for (;;) {
        sleep(1);
        if (!wd_tick()) continue;
        pthread_kill(game_thread, SIGUSR1);
    }
    return 0;
}
void port_watchdog_start(void) {
    struct sigaction sa; memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = on_usr1; sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sigaction(SIGUSR1, &sa, 0);
    game_thread = pthread_self();
    pthread_t t;
    if (pthread_create(&t, 0, wd_main, 0) == 0) pthread_detach(t);
    else port_printf("[port2] watchdog: pthread_create failed\n");
}
#endif
