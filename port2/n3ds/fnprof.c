/* PORT_FN_PROFILE (a diagnostic 3DS build: CMake -DPORT_FN_PROFILE=ON): where the main thread's time goes on the
 * console itself.  The game and the runtime are built with -finstrument-functions: every function's entry and exit
 * push and pop its address on a stack of the calling thread (thread-local, so the panel's and the mixer's threads
 * keep their own).  A thread of higher priority on the main thread's core wakes every millisecond and counts the
 * function on top of the main thread's stack (the one it is in, its callees not instrumented counted with it).
 * Every 10 seconds the log gets the 40 functions most often found, their addresses (named with the build's ELF:
 * arm-none-eabi-addr2line -f -e tombi.elf ADDR), and the samples' total.  The hooks cost time themselves: the
 * shares are what counts, not the frame rate of such a build. */
#ifdef PORT_FN_PROFILE
#include <3ds.h>
#include <string.h>
#include "port_runtime.h"

#define DEPTH 64   /* (a ring: an unbalanced enter / exit cannot run off it) */
/* the game's tasks are coroutines on the main thread (runtime/threads.c), each with its own stack of calls: one
   stack of names each (port_fnprof_switch), [9] for every other thread's calls (not sampled) */
#define NCO 10
static void *co_stack[NCO][DEPTH];
static int co_sp[NCO];
static volatile int co_cur;           /* the coroutine the main thread runs */
static __thread int fp_main;          /* 1 on the main thread */

void __cyg_profile_func_enter(void *fn, void *cs) __attribute__((no_instrument_function));
void __cyg_profile_func_exit(void *fn, void *cs) __attribute__((no_instrument_function));
void __cyg_profile_func_enter(void *fn, void *cs) {
    (void)cs; int c = fp_main ? co_cur : NCO - 1; int s = co_sp[c];
    co_stack[c][s & (DEPTH - 1)] = fn; co_sp[c] = s + 1;
}
void __cyg_profile_func_exit(void *fn, void *cs) { (void)fn; (void)cs; int c = fp_main ? co_cur : NCO - 1; co_sp[c]--; }
void port_fnprof_switch(int to) __attribute__((no_instrument_function));
void port_fnprof_switch(int to) { co_cur = to >= 0 && to < NCO - 1 ? to : NCO - 2; }   /* (runtime/threads.c switch_to) */

#define NH 2048
static struct { void *fn; unsigned n; } hist[NH];
static unsigned total, outside;

static void note(void *fn) __attribute__((no_instrument_function));
static void note(void *fn) {
    unsigned h = ((unsigned)(uintptr_t)fn >> 2) * 2654435761u >> 21;
    for (unsigned k = 0; k < NH; k++, h = (h + 1) & (NH - 1)) {
        if (hist[h].fn == fn) { hist[h].n++; return; }
        if (!hist[h].fn) { hist[h].fn = fn; hist[h].n = 1; return; }
    }
}
static void report(void) __attribute__((no_instrument_function));
static void report(void) {
    port_printf("[prof] %u samples of the main thread (1 ms apart), %u outside any instrumented function; the top 40:\n", total, outside);
    for (int r = 0; r < 40; r++) {
        int b = -1;
        for (int k = 0; k < NH; k++) if (hist[k].fn && (b < 0 || hist[k].n > hist[b].n)) b = k;
        if (b < 0 || !hist[b].n) break;
        port_printf("[prof] %5.1f%% %08x\n", 100.0 * hist[b].n / (total ? total : 1), (unsigned)(uintptr_t)hist[b].fn);
        hist[b].n = 0;
    }
    memset(hist, 0, sizeof hist); total = outside = 0;
}
static void sampler(void *arg) __attribute__((no_instrument_function));
static void sampler(void *arg) {
    (void)arg;
    for (;;) {
        svcSleepThread(1000000);
        int c = co_cur, s = co_sp[c];
        if (s > 0) note(co_stack[c][(s - 1) & (DEPTH - 1)]); else outside++;
        if (++total >= 10000) report();
    }
}
/* (called by the main thread once, before the game: n3ds/main_3ds.c) */
void port_fnprof_start(void) __attribute__((no_instrument_function));
void port_fnprof_start(void) {
    fp_main = 1;
    s32 prio = 0x30; svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
    Thread t = threadCreate(sampler, 0, 16 * 1024, prio > 0x19 ? prio - 8 : 0x18, 0, true);
    port_printf("[prof] function profile: %s (sampler priority %d, the main thread's %d)\n", t ? "on" : "no thread", (int)(prio > 0x19 ? prio - 8 : 0x18), (int)prio);
}
#else
typedef int port_fnprof_none;
#endif
