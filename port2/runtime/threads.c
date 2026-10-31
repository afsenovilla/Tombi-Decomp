/* Psy-Q BIOS threads (OpenTh / ChangeTh / CloseTh) as cooperative coroutines on native stacks.
 * The game runs its tasks as BIOS threads (src/ThreadTableRun.c, ThreadWaitFrames.c, exitTask.c): the main loop
 * switches to each task with ChangeTh(handle) and a task yields with ChangeTh(0xFF000000), the main thread.
 * The PS1 stack the game passes to OpenTh is ignored; every coroutine gets its own native stack instead. */
#include "port_runtime.h"

#define MAX_TH 8
#define STACK_WORDS (256 * 1024 / 4)

typedef struct { int used; unsigned long *sp; void (*fn)(void); } Th;
static Th th[MAX_TH + 1];                 /* [0] = main thread (handle 0xFF000000) */
static unsigned long stacks[MAX_TH][STACK_WORDS] __attribute__((aligned(16)));
static int cur;
int port_thread_current(void) { return cur; }     /* 0 = main thread (save states are taken there) */

void port_ctx_switch(unsigned long **save, unsigned long *to);
#define S_(x) #x
#define S(x) S_(x)
#define SYM(x) S(__USER_LABEL_PREFIX__) x     /* "_" prefix on Win32 */
#if defined(__arm__)
/* AAPCS: r4-r11, lr and d8-d15 are callee-saved: 9 core + 16 VFP words below the saved sp */
__asm__(".text\n.syntax unified\n.arm\n.align 2\n.globl port_ctx_switch\n.type port_ctx_switch,%function\nport_ctx_switch:\n"
        "\tpush {r4-r11, lr}\n\tvpush {d8-d15}\n"
        "\tstr sp, [r0]\n\tmov sp, r1\n"
        "\tvpop {d8-d15}\n\tpop {r4-r11, pc}\n");
#else
__asm__(".text\n.globl " SYM("port_ctx_switch") "\n" SYM("port_ctx_switch") ":\n"
        "\tmovl 4(%esp), %eax\n\tmovl 8(%esp), %edx\n"
        "\tpushl %ebp\n\tpushl %ebx\n\tpushl %esi\n\tpushl %edi\n"
        "\tmovl %esp, (%eax)\n\tmovl %edx, %esp\n"
        "\tpopl %edi\n\tpopl %esi\n\tpopl %ebx\n\tpopl %ebp\n\tret\n");
#endif

#ifdef _WIN32
/* Windows keeps the stack bounds of the thread in the TIB (fs:[4] base, fs:[8] limit) and the exception dispatcher
   checks them: on a switch they follow the coroutine's stack (the main thread's are saved on the first switch) */
static unsigned long tib_base[MAX_TH + 1], tib_limit[MAX_TH + 1];
static void tib_set(int from, int n) {
    if (!tib_base[1])
        for (int i = 1; i <= MAX_TH; i++) { tib_limit[i] = (unsigned long)stacks[i - 1]; tib_base[i] = tib_limit[i] + sizeof stacks[0]; }
    /* the main thread's limit moves as its stack grows (the kernel commits it a guard page at a time and lowers
       fs:[8]): take it again on every switch away, never restore a stale one */
    if (from == 0) __asm__ volatile("movl %%fs:4, %0\n\tmovl %%fs:8, %1" : "=r"(tib_base[0]), "=r"(tib_limit[0]));
    __asm__ volatile("movl %0, %%fs:4\n\tmovl %1, %%fs:8" : : "r"(tib_base[n]), "r"(tib_limit[n]));
}
#else
#define tib_set(from, n) ((void)0)
#endif

/* where each coroutine last yielded (the game call sites above ChangeTh on its stack, newest first): the
   "no game progress" watchdog report prints them, so a task stuck in a wait that never ends names the wait */
unsigned long port_thread_yield_site[MAX_TH + 1][4];
unsigned port_thread_switches;
static void note_yield(int from) {
    unsigned long *ebp = __builtin_frame_address(0), lo = (unsigned long)stacks[from - 1], hi = lo + sizeof stacks[0];
    int k = 0;
    for (int i = 0; i < 8 && k < 4; i++) {
        if ((unsigned long)ebp < lo || (unsigned long)ebp >= hi - 8) break;
        unsigned long ra = ebp[1], *next = (unsigned long *)ebp[0];
        if (i >= 1) port_thread_yield_site[from][k++] = ra;    /* [0] is port_thread_change's own caller: the ChangeTh shim */
        if (next <= ebp) break;
        ebp = next;
    }
    while (k < 4) port_thread_yield_site[from][k++] = 0;
}
int port_thread_used(int n) { return n >= 1 && n <= MAX_TH && th[n].used; }

static void switch_to(int n) {
    int from = cur;
    if (n == from) return;
    if (from >= 1 && from <= MAX_TH) note_yield(from);
    port_thread_switches++;
    cur = n;
    tib_set(from, n);
    port_ctx_switch(&th[from].sp, th[n].sp);
}

static void entry(void) {
    th[cur].fn();
    /* a task function returned: the BIOS would run off the end; treat it as exitTask */
    port_printf("[port] thread %d returned\n", cur);
    th[cur].used = 0;
    switch_to(0);
    port_fatal("dead thread resumed");
}

int port_thread_open(void *fn) {
    for (int i = 1; i <= MAX_TH; i++) {
        if (th[i].used) continue;
        unsigned long *sp = stacks[i - 1] + STACK_WORDS - 4;
        th[i].used = 1;
        th[i].fn = (void (*)(void))fn;      /* a native function pointer (the game names its task entry) */
#if defined(__arm__)
        sp -= 2;                            /* (keeps sp 8-byte aligned at entry(): 4 + 9 + 16 words below) */
        *--sp = (unsigned long)entry;       /* lr: popped into pc by port_ctx_switch */
        for (int k = 0; k < 8 + 16; k++) *--sp = 0;   /* r4-r11, d8-d15 */
#else
        *--sp = 0;                          /* fake return address of entry() */
        *--sp = (unsigned long)entry;       /* ret of port_ctx_switch */
        *--sp = 0; *--sp = 0; *--sp = 0; *--sp = 0;   /* ebp ebx esi edi */
#endif
        th[i].sp = sp;
        return 0xFF000000 | i;
    }
    port_fatal("OpenTh: out of threads");
}

void port_thread_change(int h) {
    int n = h & 0xFF;
    port_trace("ChangeTh", n);
    if (n > MAX_TH || (n && !th[n].used)) { port_printf("[port] ChangeTh(%08x): no such thread\n", h); return; }
    switch_to(n);
}

void port_thread_close(int h) {
    int n = h & 0xFF;
    if (n >= 1 && n <= MAX_TH) th[n].used = 0;
}

/* The game writes the status register field of the BIOS TCBs (kernel table pointer at 0x110): the zeroed BIOS
   area of the data image serves as the table; the coroutines above do not need it. */
void *port_bios_tcb_table(void) { return port_bios; }

/* --test-null, second half: the fault handlers from a coroutine stack.  A null read, a null write and a
   division by zero where the game's tasks do them: ESP inside one of the stacks above, not the main thread's
   (Windows: the TIB limits must follow the switch, or the exception dispatcher refuses the stack). */
static volatile int co_result = -1;
static void co_test(void) {
    volatile unsigned char *p = (volatile unsigned char *)0x1238; volatile int zero = 0, q, r;
    volatile unsigned int *w = (volatile unsigned int *)0x1240;
    ((unsigned char *)port_ram)[0x1238] = 0x33;
    r = *p;                     /* read: the byte the main-thread test left */
    *p = 0xA5;                  /* write */
    *w = 0xDEADBEEF;            /* 32-bit write */
    q = 7 / zero;               /* R3000: -1 */
    int ok = r == 0x33 && ((unsigned char *)port_ram)[0x1238] == 0xA5 && *(unsigned int *)((unsigned char *)port_ram + 0x1240) == 0xDEADBEEFu && q == -1;
    port_printf("[port2] test-null (coroutine %d): read 0x%02X, wrote 0xA5 -> 0x%02X, 32-bit write -> %08X, 7/0 = %d: %s\n",
                cur, r, ((unsigned char *)port_ram)[0x1238], *(unsigned int *)((unsigned char *)port_ram + 0x1240), q, ok ? "ok" : "FAILED");
    co_result = !ok;
    port_thread_change(0xFF000000);
    port_fatal("test coroutine resumed");
}
int port_test_null_coroutine(void) {
    int h = port_thread_open((void *)co_test);
    port_thread_change(h);
    port_thread_close(h);
    return co_result;
}
