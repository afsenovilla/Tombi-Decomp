/* Accesses to the BIOS kernel area through null pointers.  The game reads (and writes) fields of objects it
 * has no pointer to yet: on the PS1 that is page 0 of RAM (the kernel area, zero as far as the game cares).
 * Here such an access faults; the SIGSEGV handler copies the faulting instruction into an executable scratch
 * page with its memory operand replaced by the absolute address of the same byte in port_ram, followed by a
 * jump back, and resumes there (one fault per access; the sites are tallied for port_low_report).  Every
 * ModRM-addressed form is covered; a string instruction has ESI/EDI moved into the image and is restarted.
 * Carried over from port/runtime/sys_common.c (port_low_emulate) and sys_linux_i386.c (on_segv).
 * SIGFPE: the x86 traps on a division by zero, the R3000 does not: LO = rs < 0 ? 1 : -1, HI = rs.
 * Windows: the same through a vectored exception handler (EXCEPTION_ACCESS_VIOLATION / INT_DIVIDE_BY_ZERO) with
 * the scratch page from VirtualAlloc; the operand decoder is shared. */
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <signal.h>
#include <sys/mman.h>
#include <ucontext.h>
#endif
#include "port_runtime.h"

#define PORT_LOW_JUMP 1
#define PORT_LOW_REGS 2
#define LOW_SITES 160
static struct { unsigned long eip; unsigned long n, seen; unsigned long first; } low_sites[LOW_SITES];
static int low_nsites; static unsigned long low_total, low_other;
volatile unsigned long port_low_count, port_low_writes;   /* every emulated access so far (reads + writes), the writes */
/* the per-second line (shims/psyq.c): the accesses since the previous call and the 3 busiest sites of them */
void port_low_second(char *buf, int size) {
    static unsigned long last_total; static int inside;
    unsigned long d = port_low_count - last_total;
    last_total = port_low_count;
    int k = snprintf(buf, size, " lowmem: %lu", d);
    if (!d || inside) return;
    inside = 1;      /* (a fault inside port_sym_near would recurse here) */
    for (int t = 0; t < 3 && k < size; t++) {
        int best = -1; unsigned long bn = 0;
        for (int i = 0; i < low_nsites; i++) { unsigned long dn = low_sites[i].n - low_sites[i].seen; if (dn > bn) { bn = dn; best = i; } }
        if (best < 0) break;
        low_sites[best].seen = low_sites[best].n;
        unsigned long off; const char *nm = port_sym_near((void *)low_sites[best].eip, &off);
        k += snprintf(buf + k, size - k, "%s%s+0x%lx->%lx:%lu", t ? " " : " top ", nm, off, low_sites[best].first, bn);
    }
    for (int i = 0; i < low_nsites; i++) low_sites[i].seen = low_sites[i].n;
    inside = 0;
}
static void low_tally(unsigned long eip, unsigned long fault, int op, int len) {
    int i;
    low_total++;
    port_low_count++;
    for (i = 0; i < low_nsites; i++) if (low_sites[i].eip == eip) break;
    if (i == low_nsites && low_nsites < LOW_SITES) { low_sites[low_nsites++].eip = eip; low_sites[i].first = fault; }
    if (i < LOW_SITES) low_sites[i].n++; else low_other++;
    if (i < LOW_SITES && (low_sites[i].n <= 3 || port_verbose > 2)) {
        unsigned long off; const char *nm = port_sym_near((void *)eip, &off);
        port_printf("[port2] kernel-area access at %08lx (%s+0x%lx) -> %08lx: op %02x len %d (frame %d)\n", eip, nm, off, fault, op, len, port_frame);
    }
}
void port_low_report(void) {
    static int done;
    if (!low_total || done) return;                     /* once: a fault inside the exit path re-entered it (AREA11 wrote 1 GB) */
    done = 1;
    port_printf("[port2] %lu emulated accesses to the BIOS kernel area (page 0, through null pointers; %lu writes), %d sites%s:\n", low_total, port_low_writes, low_nsites, low_other ? " (+more)" : "");
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
/* the length of the `div`/`idiv` at p (F7 /6, F7 /7), 0 if it is something else */
static int div_len(const unsigned char *p) {
    int len = 0;
    if (p[0] == 0xF7) {
        unsigned char modrm = p[1]; int mod = modrm >> 6, rm = modrm & 7;
        len = 2;
        if (mod != 3 && rm == 4) len++;
        if (mod == 1) len += 1; else if (mod == 2) len += 4;
        else if (mod == 0 && rm == 5) len += 4;
        else if (mod == 0 && rm == 4 && (p[2] & 7) == 5) len += 4;
    }
    return len;
}

/* A call through a data pointer into the overlay region (a MAIN0 table word relocated as data because its table
   holds data pointers too, or an overlay word at no function start of its own) lands in port_ram, which is not
   executable: the fault at eip == the fault address is turned into the dispatch the thunk would have done (the
   return address and the arguments are on the stack as for any call).  Slow (a fault per call), so emit_data and
   the relocator keep giving thunks to what they can tell is a function. */
static int exec_calls; static unsigned long exec_addr[16]; static int exec_n;
void *port_exec_from_ram(unsigned long eip, unsigned long fault) {
    if (eip != fault || !port_is_ram_ptr((void *)eip)) return 0;
    u32p retail = 0x80000000u + (u32p)(eip - (unsigned long)port_ram);
    if (retail < PORT_OVL_BASE || retail >= PORT_OVL_END) return 0;
    void *fn = port_resolve_call(retail);
    if (!fn || port_is_missing(fn)) return 0;
    exec_calls++;
    int seen = 0;
    for (int i = 0; i < exec_n; i++) if (exec_addr[i] == retail) seen = 1;
    if (!seen && exec_n < 16) { exec_addr[exec_n++] = retail;
        port_printf("[port2] call into RAM at %08x (frame %d): dispatched to %s through the fault handler\n", retail, port_frame, port_sym_near(fn, 0)); }
    return fn;
}
int port_exec_from_ram_calls(void) { return exec_calls; }

#ifdef _WIN32
extern char __ImageBase[];
void port_dump_stack(unsigned long esp) {
    const unsigned long *sp = (const unsigned long *)esp;
    IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)(__ImageBase + ((IMAGE_DOS_HEADER *)__ImageBase)->e_lfanew);
    unsigned long base = (unsigned long)__ImageBase, end = base + nt->OptionalHeader.SizeOfImage;
    for (int i = 0, n = 0; i < 512 && n < 12; i++)
        if (sp[i] >= base && sp[i] < end) {
            unsigned long off; const char *nm = port_sym_near((void *)sp[i], &off);
            port_printf(" %08lx(%s+0x%lx)", sp[i], nm, off); n++;
        }
}
static LONG WINAPI on_exception(EXCEPTION_POINTERS *ep) {
    CONTEXT *c = ep->ContextRecord; EXCEPTION_RECORD *rec = ep->ExceptionRecord;
    unsigned long eip = c->Eip;
    if (rec->ExceptionCode == EXCEPTION_INT_DIVIDE_BY_ZERO) {
        int len = div_len((const unsigned char *)eip);
        if (!len) port_fatal("divide by zero at %08lx", eip);
        long rs = (long)c->Eax;
        c->Edx = (DWORD)rs; c->Eax = rs < 0 ? 1 : (DWORD)-1; c->Eip += len;
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    if (rec->ExceptionCode != EXCEPTION_ACCESS_VIOLATION) return EXCEPTION_CONTINUE_SEARCH;
    unsigned long fault = (unsigned long)rec->ExceptionInformation[1];
    { void *fn = port_exec_from_ram(eip, fault); if (fn) { c->Eip = (DWORD)(unsigned long)fn; return EXCEPTION_CONTINUE_EXECUTION; } }
    if (eip != fault && fault < 0x10000ul && low_scratch) {
        unsigned long neweip;
        int r = port_low_emulate(eip, fault, low_scratch, &neweip);
        if (r && rec->ExceptionInformation[0] == 1) port_low_writes++;
        if (r == PORT_LOW_JUMP) { c->Eip = neweip; return EXCEPTION_CONTINUE_EXECUTION; }
        if (r == PORT_LOW_REGS) {
            if (c->Esi < 0x10000ul) c->Esi += (unsigned long)port_ram;
            if (c->Edi < 0x10000ul) c->Edi += (unsigned long)port_ram;
            return EXCEPTION_CONTINUE_EXECUTION;
        }
    }
    {
        unsigned long off; const char *nm = port_sym_near((void *)eip, &off);
        port_printf("[port2] access violation: eip=%08lx (%s+0x%lx) fault address=%08lx (%s, frame %d, thread %d, image base %p); stack:", eip, nm, off, fault,
                    rec->ExceptionInformation[0] == 8 ? "execute" : rec->ExceptionInformation[0] ? "write" : "read", port_frame, port_thread_current(), (void *)__ImageBase);
        port_dump_stack(c->Esp);
        port_printf("\n");
    }
    port_low_report();
    port_window_stop();
    fflush(stderr);
    return EXCEPTION_CONTINUE_SEARCH;
}
void port_lowmem_install(void) {
    low_scratch = VirtualAlloc(0, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!low_scratch) port_printf("[port2] no executable scratch page: null-pointer accesses will crash\n");
    AddVectoredExceptionHandler(1, on_exception);
}
#else
void port_dump_stack(unsigned long esp) {
    const unsigned long *sp = (const unsigned long *)esp;
    extern char __executable_start[], etext[];
    for (int i = 0, n = 0; i < 512 && n < 12; i++)
        if (sp[i] >= (unsigned long)__executable_start && sp[i] < (unsigned long)etext) {
            unsigned long off; const char *nm = port_sym_near((void *)sp[i], &off);
            port_printf(" %08lx(%s+0x%lx)", sp[i], nm, off); n++;
        }
}
static void on_segv(int sig, siginfo_t *info, void *ucv) {
    ucontext_t *uc = ucv;
    unsigned long eip = uc->uc_mcontext.gregs[REG_EIP], fault = (unsigned long)info->si_addr;
    { void *fn = port_exec_from_ram(eip, fault); if (fn) { uc->uc_mcontext.gregs[REG_EIP] = (long)fn; return; } }
    if (eip != fault && fault < 0x10000ul && low_scratch) {
        unsigned long neweip;
        int r = port_low_emulate(eip, fault, low_scratch, &neweip);
        if (r && (uc->uc_mcontext.gregs[REG_ERR] & 2)) port_low_writes++;     /* page-fault error code bit 1: a write */
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
        port_dump_stack(uc->uc_mcontext.gregs[REG_ESP]);
        port_printf("\n");
    }
    port_low_report();
    signal(sig, SIG_DFL);
    raise(sig);
}
static void on_fpe(int sig, siginfo_t *info, void *ucv) {
    ucontext_t *uc = ucv;
    unsigned char *p = (unsigned char *)uc->uc_mcontext.gregs[REG_EIP];
    int len = div_len(p);
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
#endif
