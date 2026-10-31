/* lowmem.c for 32-bit ARM (Android armeabi-v7a).  Included by lowmem.c instead of the Linux i386 handler.
 *
 * Null-pointer accesses into page 0 (the BIOS kernel area): the game code is built with -marm (port2/CMakeLists.txt),
 * so the faulting instruction is an A32 load/store.  The handler decodes it, does the access on the same bytes of
 * port_ram, writes the loaded register(s) and the base writeback into the ucontext and steps over it (no scratch page:
 * nothing is executed out of line).  Covered: LDR/STR/LDRB/STRB (immediate and shifted-register offset),
 * LDRH/STRH/LDRSB/LDRSH/LDRD/STRD, LDM/STM.  Not covered (reported as a crash): VFP/NEON loads (VLDR/VLDM), Thumb code
 * (SDL, libc: never game code), exclusive loads.
 * Division by zero: ARMv7-A without the integer divide extension calls __aeabi_idiv & co.; the definitions below
 * (bound inside libtombi.so by -Bsymbolic) give the R3000's result for a zero divisor (LO = rs < 0 ? 1 : -1, HI = rs;
 * unsigned: LO = 0xFFFFFFFF, HI = rs), otherwise the ordinary quotient. */
#include <unistd.h>
#include <sys/syscall.h>

/* (compiler-rt defines __aeabi_idiv/__aeabi_uidiv as aliases inside divsi3.S/udivsi3.S, so these cannot call
   __divsi3/__udivsi3 without pulling in a duplicate: plain shift-subtract division instead) */
static unsigned udiv32(unsigned a, unsigned b, unsigned *rem) {
    unsigned q = 0, r = 0;
    for (int i = 31; i >= 0; i--) {
        r = (r << 1) | ((a >> i) & 1);
        if (r >= b) { r -= b; q |= 1u << i; }
    }
    *rem = r;
    return q;
}
static int sdiv32(int a, int b, int *rem) {
    unsigned ua = a < 0 ? 0u - (unsigned)a : (unsigned)a, ub = b < 0 ? 0u - (unsigned)b : (unsigned)b, r;
    unsigned q = udiv32(ua, ub, &r);
    *rem = a < 0 ? -(int)r : (int)r;
    return (a < 0) != (b < 0) ? -(int)q : (int)q;
}
int __aeabi_idiv(int a, int b) { int r; return b ? sdiv32(a, b, &r) : (a < 0 ? 1 : -1); }
unsigned __aeabi_uidiv(unsigned a, unsigned b) { unsigned r; return b ? udiv32(a, b, &r) : 0xFFFFFFFFu; }
/* {quotient, remainder} in r0:r1 (AAPCS returns a 64-bit integer there, low word in r0) */
unsigned long long __aeabi_idivmod(int a, int b) {
    int r = a, q = b ? sdiv32(a, b, &r) : (a < 0 ? 1 : -1);
    return (unsigned)q | (unsigned long long)(unsigned)r << 32;
}
unsigned long long __aeabi_uidivmod(unsigned a, unsigned b) {
    unsigned r = a, q = b ? udiv32(a, b, &r) : 0xFFFFFFFFu;
    return q | (unsigned long long)r << 32;
}

void port_dump_stack(unsigned long esp) {
    const unsigned long *sp = (const unsigned long *)esp;
    extern char __ehdr_start[], etext[];      /* (lld's: the start of libtombi.so and the end of its text) */
    for (int i = 0, n = 0; i < 512 && n < 12; i++)
        if (sp[i] >= (unsigned long)__ehdr_start && sp[i] < (unsigned long)etext) {
            unsigned long off; const char *nm = port_sym_near((void *)sp[i], &off);
            port_printf(" %08lx(%s+0x%lx)", sp[i], nm, off); n++;
        }
}

static unsigned long arm_shift(unsigned long v, int type, int amt, unsigned long cpsr) {
    switch (type) {
    case 0: return amt ? v << amt : v;                                           /* LSL */
    case 1: return amt ? v >> amt : 0;                                           /* LSR #32 */
    case 2: return amt ? (unsigned long)((long)v >> amt) : ((long)v < 0 ? ~0ul : 0);   /* ASR */
    default: return amt ? (v >> amt) | (v << (32 - amt)) : (((cpsr >> 29) & 1) << 31) | (v >> 1);   /* ROR / RRX */
    }
}
static int low_ok(unsigned long ea, int n) { return ea < 0x10000ul && ea + n <= 0x10000ul; }
static unsigned long low_rd(unsigned long ea, int n) {
    const unsigned char *m = (const unsigned char *)port_ram + ea;
    return n == 1 ? m[0] : n == 2 ? (unsigned long)(m[0] | m[1] << 8) : (unsigned long)(m[0] | m[1] << 8 | m[2] << 16 | (unsigned long)m[3] << 24);
}
static void low_wr(unsigned long ea, int n, unsigned long v) {
    unsigned char *m = (unsigned char *)port_ram + ea;
    for (int i = 0; i < n; i++) m[i] = (unsigned char)(v >> (8 * i));
}

/* r[0..15]: the saved r0-r15 of the ucontext (arm_r0 ... arm_pc are consecutive).  1 = emulated (pc advanced). */
static int arm_low_emulate(unsigned long *r, unsigned long cpsr, unsigned long fault) {
    unsigned long pc = r[15], insn = *(const unsigned long *)pc;
    int P = (insn >> 24) & 1, U = (insn >> 23) & 1, W = (insn >> 21) & 1, L = (insn >> 20) & 1;
    int rn = (insn >> 16) & 15, rt = (insn >> 12) & 15;
    unsigned long base = rn == 15 ? pc + 8 : r[rn], off, ea, wb;
    if (cpsr & 0x20) return 0;                                                  /* Thumb: not game code */
    if ((insn >> 28) == 15) return 0;                                           /* unconditional space (PLD, NEON) */
    if (((insn >> 26) & 3) == 1) {                                              /* LDR/STR/LDRB/STRB */
        int B = (insn >> 22) & 1, n = B ? 1 : 4;
        if (insn & (1u << 25)) {
            if (insn & 0x10) return 0;                                          /* (media instructions) */
            off = arm_shift(r[insn & 15], (insn >> 5) & 3, (insn >> 7) & 31, cpsr);
        } else off = insn & 0xFFF;
        wb = U ? base + off : base - off; ea = P ? wb : base;
        if (!low_ok(ea, n) || rt == 15) return 0;
        if (L) { unsigned long v = low_rd(ea, n); if (!P || W) r[rn] = wb; r[rt] = v; }
        else { low_wr(ea, n, rt == 15 ? pc + 8 : r[rt]); if (!P || W) r[rn] = wb; }
        low_tally(pc, fault, (insn >> 20) & 0xFF, 4);
        if (!L) port_low_writes++;
        r[15] = pc + 4;
        return 1;
    }
    if (((insn >> 25) & 7) == 0 && (insn & 0x90) == 0x90 && (insn & 0x60)) {   /* LDRH/STRH/LDRSB/LDRSH/LDRD/STRD */
        int sh = (insn >> 5) & 3, n, sign = 0, dual = 0, load = L;
        off = (insn & (1u << 22)) ? (((insn >> 4) & 0xF0) | (insn & 15)) : r[insn & 15];
        if (L) { n = sh == 1 ? 2 : sh == 2 ? 1 : 2; sign = sh != 1; }
        else if (sh == 1) n = 2;
        else { n = 4; dual = 1; load = sh == 2; }                               /* LDRD (sh 2) / STRD (sh 3) */
        wb = U ? base + off : base - off; ea = P ? wb : base;
        if (!low_ok(ea, dual ? 8 : n) || rt == 15 || (dual && (rt & 1))) return 0;
        if (load) {
            unsigned long v = low_rd(ea, n);
            if (sign) v = n == 1 ? (unsigned long)(long)(signed char)v : (unsigned long)(long)(short)v;
            unsigned long v2 = dual ? low_rd(ea + 4, 4) : 0;
            if (!P || W) r[rn] = wb;
            r[rt] = v; if (dual) r[rt + 1] = v2;
        } else {
            low_wr(ea, n, r[rt]); if (dual) low_wr(ea + 4, 4, r[rt + 1]);
            if (!P || W) r[rn] = wb;
            port_low_writes++;
        }
        low_tally(pc, fault, (insn >> 20) & 0xFF, 4);
        r[15] = pc + 4;
        return 1;
    }
    if (((insn >> 25) & 7) == 4 && !(insn & (1u << 22))) {                      /* LDM/STM (no user-bank form) */
        unsigned list = insn & 0xFFFF; int cnt = __builtin_popcount(list);
        if (!cnt || rn == 15) return 0;
        unsigned long start = U ? base + (P ? 4 : 0) : base - 4ul * cnt + (P ? 0 : 4);
        if (!low_ok(start, 4 * cnt)) return 0;
        unsigned long vals[16], a = start, newpc = pc + 4;
        for (int i = 0; i < 16; i++) if (list & (1u << i)) {
            if (L) vals[i] = low_rd(a, 4); else low_wr(a, 4, i == 15 ? pc + 8 : r[i]);
            a += 4;
        }
        if (W) r[rn] = U ? base + 4ul * cnt : base - 4ul * cnt;
        if (L) { for (int i = 0; i < 16; i++) if (list & (1u << i)) { if (i == 15) newpc = vals[i]; else r[i] = vals[i]; } }
        else port_low_writes++;
        low_tally(pc, fault, (insn >> 20) & 0xFF, 4);
        r[15] = newpc;
        return 1;
    }
    return 0;
}

static void on_segv(int sig, siginfo_t *info, void *ucv) {
    ucontext_t *uc = ucv;
    unsigned long *r = (unsigned long *)&uc->uc_mcontext.arm_r0;
    unsigned long eip = uc->uc_mcontext.arm_pc, fault = (unsigned long)info->si_addr;
    { void *fn = port_exec_from_ram(eip, fault);
      if (fn) { uc->uc_mcontext.arm_pc = (unsigned long)fn & ~1ul;
                if ((unsigned long)fn & 1) uc->uc_mcontext.arm_cpsr |= 0x20; else uc->uc_mcontext.arm_cpsr &= ~0x20ul;
                return; } }
    if (eip != fault && fault < 0x10000ul && arm_low_emulate(r, uc->uc_mcontext.arm_cpsr, fault)) return;
    {
        static volatile sig_atomic_t reporting;
        char raw[64]; unsigned n = raw_fault_line(raw, "[port2] fault: pc=", eip, fault);
        syscall(SYS_write, 2, raw, n);           /* (the game has its own `write`, a stub) */
        if (reporting) { signal(sig, SIG_DFL); raise(sig); return; }
        reporting = 1;
        unsigned long off; const char *nm = port_sym_near((void *)eip, &off);
        port_printf("[port2] signal %d: pc=%08lx (%s+0x%lx) fault address=%08lx (frame %d); stack:", sig, eip, nm, off, fault, port_frame);
        port_dump_stack(uc->uc_mcontext.arm_sp);
        port_printf("\n");
    }
    port_low_report();
    signal(sig, SIG_DFL);
    raise(sig);
}
static unsigned char alt_stack[65536];
void port_lowmem_install(void) {
    stack_t ss = { alt_stack, 0, sizeof alt_stack };
    sigaltstack(&ss, 0);
    struct sigaction sa; memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = on_segv; sa.sa_flags = SA_SIGINFO | SA_NODEFER | SA_ONSTACK;
    sigaction(SIGSEGV, &sa, 0); sigaction(SIGBUS, &sa, 0);
}
