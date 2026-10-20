/* Accesses to the BIOS kernel area through null pointers.  The game reads (and writes) fields of objects it
 * has no pointer to yet: on the PS1 that is page 0 of RAM (the kernel area, zero as far as the game cares).
 * Here such an access faults; the SIGSEGV handler copies the faulting instruction into an executable scratch
 * page with its memory operand replaced by the absolute address of the same byte in port_ram, followed by a
 * jump back, and resumes there (one fault per access; the sites are tallied for port_low_report).  Every
 * ModRM-addressed form is covered; a string instruction has ESI/EDI moved into the image and is restarted.
 * Carried over from port/runtime/sys_common.c (port_low_emulate) and sys_linux_i386.c (on_segv).
 * SIGFPE: the x86 traps on a division by zero, the R3000 does not: LO = rs < 0 ? 1 : -1, HI = rs. */
#define _GNU_SOURCE
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <ucontext.h>
#include <execinfo.h>
#include "port_runtime.h"

#define PORT_LOW_JUMP 1
#define PORT_LOW_REGS 2
#define LOW_SITES 160
static struct { unsigned long eip; unsigned long n; unsigned long first; } low_sites[LOW_SITES];
static int low_nsites; static unsigned long low_total, low_other;
static void low_tally(unsigned long eip, unsigned long fault, int op, int len) {
    int i;
    low_total++;
    for (i = 0; i < low_nsites; i++) if (low_sites[i].eip == eip) break;
    if (i == low_nsites && low_nsites < LOW_SITES) { low_sites[low_nsites++].eip = eip; low_sites[i].first = fault; }
    if (i < LOW_SITES) low_sites[i].n++; else low_other++;
    if (i < LOW_SITES && (low_sites[i].n <= 3 || port_verbose > 2)) {
        unsigned long off; const char *nm = port_sym_near((void *)eip, &off);
        port_printf("[port2] kernel-area access at %08lx (%s+0x%lx) -> %08lx: op %02x len %d (frame %d)\n", eip, nm, off, fault, op, len, port_frame);
    }
}
void port_low_report(void) {
    if (!low_total) return;
    port_printf("[port2] %lu emulated accesses to the BIOS kernel area (page 0, through null pointers), %d sites%s:\n", low_total, low_nsites, low_other ? " (+more)" : "");
    for (int i = 0; i < low_nsites; i++) {
        unsigned long off; const char *nm = port_sym_near((void *)low_sites[i].eip, &off);
        port_printf("  %10lu  %08lx %s+0x%lx (first address %08lx)\n", low_sites[i].n, low_sites[i].eip, nm, off, low_sites[i].first);
    }
}
int port_low_emulate(unsigned long eip, unsigned long fault, unsigned char *scratch, unsigned long *neweip) {
    const unsigned char *p = (const unsigned char *)eip;
    int i = 0, opsize16 = 0;
    unsigned char pre[4]; int npre = 0;
    while (i < 4 && (p[i] == 0x66 || p[i] == 0xF0 || p[i] == 0xF2 || p[i] == 0xF3 || p[i] == 0x2E || p[i] == 0x3E || p[i] == 0x26 || p[i] == 0x36 || p[i] == 0x64 || p[i] == 0x65)) {
        if (p[i] == 0x66) opsize16 = 1;
        pre[npre++] = p[i++];
    }
    unsigned char op[2]; int nop = 1;
    op[0] = p[i++];
    if (op[0] == 0x0F) { op[1] = p[i++]; nop = 2; }
    if (nop == 1 && op[0] >= 0xA4 && op[0] <= 0xAF && op[0] != 0xA8 && op[0] != 0xA9) {   /* string instructions: ESI/EDI implicit */
        low_tally(eip, fault, op[0], i);
        return PORT_LOW_REGS;
    }
    if (nop == 1 && (op[0] >= 0xA0 && op[0] <= 0xA3)) return 0;         /* mov moffs: an absolute address, never low */
    unsigned char modrm = p[i++];
    int mod = modrm >> 6, reg = (modrm >> 3) & 7, rm = modrm & 7;
    if (mod == 3) return 0;
    if (rm == 4) i++;                                                     /* SIB */
    if (mod == 1) i += 1; else if (mod == 2) i += 4;
    else if (mod == 0 && rm == 5) i += 4;
    else if (mod == 0 && rm == 4 && (p[i - 1] & 7) == 5) i += 4;
    int imm = 0;
    if (nop == 1) {
        switch (op[0]) {
        case 0x80: case 0x82: case 0x83: case 0xC0: case 0xC1: case 0xC6: case 0x6B: imm = 1; break;
        case 0x81: case 0x69: case 0xC7: imm = opsize16 ? 2 : 4; break;
        case 0xF6: imm = reg <= 1 ? 1 : 0; break;
        case 0xF7: imm = reg <= 1 ? (opsize16 ? 2 : 4) : 0; break;
        case 0x8D: return 0;                                              /* lea does not access memory */
        }
    } else if (op[1] == 0xBA || op[1] == 0xA4 || op[1] == 0xAC || op[1] == 0xC2 || (op[1] >= 0x70 && op[1] <= 0x73)) imm = 1;
    const unsigned char *immp = p + i;
    i += imm;
    unsigned long target = (unsigned long)port_ram + fault;   /* the same byte of the kernel area in the data image */
    unsigned char *o = scratch; int k = 0;
    for (int j = 0; j < npre; j++) o[k++] = pre[j];
    for (int j = 0; j < nop; j++) o[k++] = op[j];
    o[k++] = (unsigned char)((reg << 3) | 5);                             /* mod 00, rm 101: [disp32] */
    o[k++] = target; o[k++] = target >> 8; o[k++] = target >> 16; o[k++] = target >> 24;
    for (int j = 0; j < imm; j++) o[k++] = immp[j];
    unsigned long next = eip + i, rel = next - ((unsigned long)o + k + 5);
    o[k++] = 0xE9; o[k++] = rel; o[k++] = rel >> 8; o[k++] = rel >> 16; o[k++] = rel >> 24;
    low_tally(eip, fault, op[nop - 1], i);
    *neweip = (unsigned long)o;
    return PORT_LOW_JUMP;
}

static unsigned char *low_scratch;
static void on_segv(int sig, siginfo_t *info, void *ucv) {
    ucontext_t *uc = ucv;
    unsigned long eip = uc->uc_mcontext.gregs[REG_EIP], fault = (unsigned long)info->si_addr;
    if (eip != fault && fault < 0x10000ul && low_scratch) {
        unsigned long neweip;
        int r = port_low_emulate(eip, fault, low_scratch, &neweip);
        if (r == PORT_LOW_JUMP) { uc->uc_mcontext.gregs[REG_EIP] = neweip; return; }
        if (r == PORT_LOW_REGS) {
            if ((unsigned long)uc->uc_mcontext.gregs[REG_ESI] < 0x10000ul) uc->uc_mcontext.gregs[REG_ESI] += (unsigned long)port_ram;
            if ((unsigned long)uc->uc_mcontext.gregs[REG_EDI] < 0x10000ul) uc->uc_mcontext.gregs[REG_EDI] += (unsigned long)port_ram;
            return;
        }
    }
    {
        unsigned long off; const char *nm = port_sym_near((void *)eip, &off);
        port_printf("[port2] signal %d: eip=%08lx (%s+0x%lx) fault address=%08lx (frame %d); stack:", sig, eip, nm, off, fault, port_frame);
        const unsigned long *sp = (const unsigned long *)uc->uc_mcontext.gregs[REG_ESP];
        extern char __executable_start[], etext[];
        for (int i = 0, n = 0; i < 512 && n < 12; i++)
            if (sp[i] >= (unsigned long)__executable_start && sp[i] < (unsigned long)etext) {
                nm = port_sym_near((void *)sp[i], &off);
                port_printf(" %08lx(%s+0x%lx)", sp[i], nm, off); n++;
            }
        port_printf("\n");
    }
    port_low_report();
    signal(sig, SIG_DFL);
    raise(sig);
}
static void on_fpe(int sig, siginfo_t *info, void *ucv) {
    ucontext_t *uc = ucv;
    unsigned char *p = (unsigned char *)uc->uc_mcontext.gregs[REG_EIP];
    int len = 0;
    if (p[0] == 0xF7) {
        unsigned char modrm = p[1]; int mod = modrm >> 6, rm = modrm & 7;
        len = 2;
        if (mod != 3 && rm == 4) len++;
        if (mod == 1) len += 1; else if (mod == 2) len += 4;
        else if (mod == 0 && rm == 5) len += 4;
        else if (mod == 0 && rm == 4 && (p[2] & 7) == 5) len += 4;
    }
    if (!len) port_fatal("SIGFPE at %p", (void *)p);
    long rs = (long)uc->uc_mcontext.gregs[REG_EAX];
    uc->uc_mcontext.gregs[REG_EDX] = rs;
    uc->uc_mcontext.gregs[REG_EAX] = rs < 0 ? 1 : -1;
    uc->uc_mcontext.gregs[REG_EIP] += len;
    (void)sig; (void)info;
}
static unsigned char alt_stack[65536];
void port_lowmem_install(void) {
    low_scratch = mmap(0, 4096, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (low_scratch == MAP_FAILED) { low_scratch = 0; port_printf("[port2] no executable scratch page: null-pointer accesses will crash\n"); }
    stack_t ss = { alt_stack, 0, sizeof alt_stack };
    sigaltstack(&ss, 0);
    struct sigaction sa; memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = on_segv; sa.sa_flags = SA_SIGINFO | SA_NODEFER | SA_ONSTACK;
    sigaction(SIGSEGV, &sa, 0); sigaction(SIGBUS, &sa, 0);
    sa.sa_sigaction = on_fpe; sa.sa_flags = SA_SIGINFO | SA_NODEFER;
    sigaction(SIGFPE, &sa, 0);
}
