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
__asm__(".text\n.globl port_ctx_switch\nport_ctx_switch:\n"
        "\tmovl 4(%esp), %eax\n\tmovl 8(%esp), %edx\n"
        "\tpushl %ebp\n\tpushl %ebx\n\tpushl %esi\n\tpushl %edi\n"
        "\tmovl %esp, (%eax)\n\tmovl %edx, %esp\n"
        "\tpopl %edi\n\tpopl %esi\n\tpopl %ebx\n\tpopl %ebp\n\tret\n");

static void switch_to(int n) {
    int from = cur;
    if (n == from) return;
    cur = n;
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
        *--sp = 0;                          /* fake return address of entry() */
        *--sp = (unsigned long)entry;       /* ret of port_ctx_switch */
        *--sp = 0; *--sp = 0; *--sp = 0; *--sp = 0;   /* ebp ebx esi edi */
        th[i].sp = sp;
        return 0xFF000000 | i;
    }
    port_fatal("OpenTh: out of threads");
}

void port_thread_change(int h) {
    int n = h & 0xFF;
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
