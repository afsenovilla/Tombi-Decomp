/* Fallback for game code that has no C yet: a small R3000 interpreter that runs the retail machine code where it
 * lies in PS1 RAM (MAIN0 from the EXE image, overlays as the game itself loaded them).
 *
 * The address space is the same for native and interpreted code, so loads and stores are plain host memory
 * accesses.  A jal/jalr to a function the port has natively (port_lookup / port_libmap: game C, shims) calls it
 * with cdecl, passing $a0-$a3 and the first four stack arguments, and takes $v0/$v1 from eax/edx.  A native
 * caller enters through port_mips_entry (call thunks, the SIGSEGV trampoline), which hands over its eight
 * argument words.  COP2 goes to the software GTE (runtime/gte.c).  Load delay slots are not modelled
 * (compiled code does not depend on them); branch delay slots are. */
#include "port_runtime.h"

typedef unsigned int u32; typedef int s32; typedef unsigned short u16; typedef short s16;
typedef unsigned char u8; typedef signed char s8; typedef unsigned long long u64; typedef long long s64;

void port_gte_op(unsigned long op);
void port_gte_mtc2(int reg, unsigned long v);
unsigned long port_gte_mfc2(int reg);
void port_gte_ctc2(int reg, unsigned long v);
unsigned long port_gte_cfc2(int reg);

#define RA_SENTINEL 0xFFFFFFF0u
#define MIPS_STACK_WORDS 4096

int port_interp = 1;
typedef u64 (*NativeFn)(u32, u32, u32, u32, u32, u32, u32, u32);
int port_mips_trace;             /* --trace-interp: log every interpreted entry */
static int calls_interp;

/* native code for a retail address: game C (funcmap / overlay set / handwritten) or a shim (libmap) */
static void *native_for(u32 a) {
    void *f = port_lookup(a);
    if (f) return f;
    if (a >= PORT_OVL_BASE && a < PORT_OVL_END) return 0;   /* overlay code: interpret what is loaded */
    int lo = 0, hi = port_libmap_count - 1;
    while (lo <= hi) {
        int m = (lo + hi) / 2;
        if (port_libmap[m].addr == a) return port_libmap[m].fn;
        if (port_libmap[m].addr < a) lo = m + 1; else hi = m - 1;
    }
    return 0;
}

/* MAIN0 library code without a shim is hardware access: never interpret it, call it a stub */
int port_is_lib_addr(u32 a) {
    int lo = 0, hi = port_libaddrs_count - 1;
    while (lo <= hi) {
        int m = (lo + hi) / 2;
        if (port_libaddrs[m] == a) return 1;
        if (port_libaddrs[m] < a) lo = m + 1; else hi = m - 1;
    }
    return 0;
}
static int is_lib_without_shim(u32 a) { return port_is_lib_addr(a) && !native_for(a); }

static u32 lwl(u32 a, u32 rt) {
    u32 w = *(u32 *)(a & ~3u); int s = a & 3;
    return (rt & (0x00FFFFFFu >> (s * 8))) | (w << (24 - s * 8));
}
static u32 lwr(u32 a, u32 rt) {
    u32 w = *(u32 *)(a & ~3u); int s = a & 3;
    return (rt & (0xFFFFFF00u << (24 - s * 8))) | (w >> (s * 8));
}
static void swl(u32 a, u32 rt) {
    u32 *p = (u32 *)(a & ~3u); int s = a & 3;
    *p = (*p & (0xFFFFFF00u << (s * 8))) | (rt >> (24 - s * 8));
}
static void swr(u32 a, u32 rt) {
    u32 *p = (u32 *)(a & ~3u); int s = a & 3;
    *p = (*p & (0x00FFFFFFu >> (24 - s * 8))) | (rt << (s * 8));
}

u32 port_mips_call(u32 entry, const u32 *args) {
    u32 stack[MIPS_STACK_WORDS];
    u32 r[32], hi = 0, lo = 0, pc = entry, npc = entry + 4;
    for (int i = 0; i < 32; i++) r[i] = 0;
    r[29] = (u32)&stack[MIPS_STACK_WORDS - 16];
    for (int i = 0; i < 4; i++) { r[4 + i] = args[i]; ((u32 *)r[29])[i] = args[i]; }
    for (int i = 4; i < 8; i++) ((u32 *)r[29])[i] = args[i];
    r[31] = RA_SENTINEL;
    calls_interp++;
    if (port_mips_trace) port_printf("[mips] enter %08x (%08x %08x %08x %08x)\n", entry, args[0], args[1], args[2], args[3]);
    for (;;) {
        if (pc == RA_SENTINEL) break;
        u32 ins = *(u32 *)pc;
        u32 cur = pc;
        pc = npc; npc += 4;
        u32 op = ins >> 26, rs = (ins >> 21) & 31, rt = (ins >> 16) & 31, rd = (ins >> 11) & 31;
        u32 sh = (ins >> 6) & 31, fn = ins & 63;
        s32 imm = (s16)ins; u32 uimm = ins & 0xFFFF;
        u32 target = 0; int link = 0, jump = 0;
        switch (op) {
        case 0x00:
            switch (fn) {
            case 0x00: r[rd] = r[rt] << sh; break;
            case 0x02: r[rd] = r[rt] >> sh; break;
            case 0x03: r[rd] = (u32)((s32)r[rt] >> sh); break;
            case 0x04: r[rd] = r[rt] << (r[rs] & 31); break;
            case 0x06: r[rd] = r[rt] >> (r[rs] & 31); break;
            case 0x07: r[rd] = (u32)((s32)r[rt] >> (r[rs] & 31)); break;
            case 0x08: target = r[rs]; jump = 1; break;                         /* jr */
            case 0x09: target = r[rs]; jump = 1; link = rd ? (int)rd : 31; break; /* jalr */
            case 0x0C: port_printf("[mips] syscall at %08x\n", cur); break;
            case 0x0D: break;                                                   /* break: division traps */
            case 0x10: r[rd] = hi; break;
            case 0x11: hi = r[rs]; break;
            case 0x12: r[rd] = lo; break;
            case 0x13: lo = r[rs]; break;
            case 0x18: { s64 m = (s64)(s32)r[rs] * (s32)r[rt]; lo = (u32)m; hi = (u32)(m >> 32); break; }
            case 0x19: { u64 m = (u64)r[rs] * r[rt]; lo = (u32)m; hi = (u32)(m >> 32); break; }
            case 0x1A: {
                s32 n = r[rs], d = r[rt];
                if (d == 0) { hi = n; lo = n < 0 ? 1 : 0xFFFFFFFFu; }
                else if (n == (s32)0x80000000 && d == -1) { hi = 0; lo = 0x80000000u; }
                else { lo = n / d; hi = n % d; }
                break;
            }
            case 0x1B:
                if (r[rt] == 0) { hi = r[rs]; lo = 0xFFFFFFFFu; } else { lo = r[rs] / r[rt]; hi = r[rs] % r[rt]; }
                break;
            case 0x20: case 0x21: r[rd] = r[rs] + r[rt]; break;
            case 0x22: case 0x23: r[rd] = r[rs] - r[rt]; break;
            case 0x24: r[rd] = r[rs] & r[rt]; break;
            case 0x25: r[rd] = r[rs] | r[rt]; break;
            case 0x26: r[rd] = r[rs] ^ r[rt]; break;
            case 0x27: r[rd] = ~(r[rs] | r[rt]); break;
            case 0x2A: r[rd] = (s32)r[rs] < (s32)r[rt]; break;
            case 0x2B: r[rd] = r[rs] < r[rt]; break;
            default: port_fatal("[mips] unknown SPECIAL %08x at %08x", ins, cur);
            }
            break;
        case 0x01: {                                    /* bltz bgez bltzal bgezal */
            int take = (rt & 1) ? (s32)r[rs] >= 0 : (s32)r[rs] < 0;
            if (rt & 0x10) r[31] = cur + 8;
            if (take) npc = pc + (imm << 2);
            break;
        }
        case 0x02: target = (pc & 0xF0000000u) | ((ins & 0x03FFFFFFu) << 2); jump = 1; break;
        case 0x03: target = (pc & 0xF0000000u) | ((ins & 0x03FFFFFFu) << 2); jump = 1; link = 31; break;
        case 0x04: if (r[rs] == r[rt]) npc = pc + (imm << 2); break;
        case 0x05: if (r[rs] != r[rt]) npc = pc + (imm << 2); break;
        case 0x06: if ((s32)r[rs] <= 0) npc = pc + (imm << 2); break;
        case 0x07: if ((s32)r[rs] > 0) npc = pc + (imm << 2); break;
        case 0x08: case 0x09: r[rt] = r[rs] + imm; break;
        case 0x0A: r[rt] = (s32)r[rs] < imm; break;
        case 0x0B: r[rt] = r[rs] < (u32)imm; break;
        case 0x0C: r[rt] = r[rs] & uimm; break;
        case 0x0D: r[rt] = r[rs] | uimm; break;
        case 0x0E: r[rt] = r[rs] ^ uimm; break;
        case 0x0F: r[rt] = uimm << 16; break;
        case 0x10: break;                               /* COP0: interrupts etc., nothing to do */
        case 0x12:                                      /* COP2 */
            if (ins & (1u << 25)) port_gte_op(ins & 0x1FFFFFFu);
            else switch (rs) {
                case 0x00: r[rt] = port_gte_mfc2(rd); break;
                case 0x02: r[rt] = port_gte_cfc2(rd); break;
                case 0x04: port_gte_mtc2(rd, r[rt]); break;
                case 0x06: port_gte_ctc2(rd, r[rt]); break;
                default: port_fatal("[mips] unknown COP2 %08x at %08x", ins, cur);
            }
            break;
        case 0x20: r[rt] = (u32)(s32)*(s8 *)(r[rs] + imm); break;
        case 0x21: r[rt] = (u32)(s32)*(s16 *)(r[rs] + imm); break;
        case 0x22: r[rt] = lwl(r[rs] + imm, r[rt]); break;
        case 0x23: r[rt] = *(u32 *)(r[rs] + imm); break;
        case 0x24: r[rt] = *(u8 *)(r[rs] + imm); break;
        case 0x25: r[rt] = *(u16 *)(r[rs] + imm); break;
        case 0x26: r[rt] = lwr(r[rs] + imm, r[rt]); break;
        case 0x28: *(u8 *)(r[rs] + imm) = (u8)r[rt]; break;
        case 0x29: *(u16 *)(r[rs] + imm) = (u16)r[rt]; break;
        case 0x2A: swl(r[rs] + imm, r[rt]); break;
        case 0x2B: *(u32 *)(r[rs] + imm) = r[rt]; break;
        case 0x2E: swr(r[rs] + imm, r[rt]); break;
        case 0x32: port_gte_mtc2(rt, *(u32 *)(r[rs] + imm)); break;   /* lwc2 */
        case 0x3A: *(u32 *)(r[rs] + imm) = port_gte_mfc2(rt); break;  /* swc2 */
        default: port_fatal("[mips] unknown opcode %08x at %08x", ins, cur);
        }
        r[0] = 0;
        if (!jump) continue;
        /* the delay slot runs first */
        {
            u32 save_pc = pc;
            if (link) r[link] = cur + 8;
            u32 tgt = target;
            /* execute the delay slot instruction by stepping once with npc forced to the target */
            pc = save_pc; npc = tgt;
            /* a call to native code (or a stub) completes right here: run the delay slot, then call */
            if (link && tgt != RA_SENTINEL) {
                void *f = 0;
                int native = (tgt < 0x80000000u || tgt >= 0x80200000u) ? 1 : 0;   /* host function pointer */
                if (!native) { f = native_for(tgt); native = f != 0 || is_lib_without_shim(tgt); }
                else f = (void *)tgt;
                if (native) {
                    /* step the delay slot alone: re-enter the loop for one instruction */
                    u32 dins = *(u32 *)save_pc;
                    if (dins != 0) {
                        /* simple, common delay-slot forms: ALU / loads / stores; reuse the decoder by a tiny
                           recursion on a one-instruction "program" ending in the sentinel would need a frame,
                           so decode here the subset compilers put in jal delay slots */
                        u32 o2 = dins >> 26, s2 = (dins >> 21) & 31, t2 = (dins >> 16) & 31, d2 = (dins >> 11) & 31;
                        s32 i2 = (s16)dins; u32 u2 = dins & 0xFFFF;
                        switch (o2) {
                        case 0x00:
                            switch (dins & 63) {
                            case 0x00: r[d2] = r[t2] << ((dins >> 6) & 31); break;
                            case 0x02: r[d2] = r[t2] >> ((dins >> 6) & 31); break;
                            case 0x03: r[d2] = (u32)((s32)r[t2] >> ((dins >> 6) & 31)); break;
                            case 0x21: case 0x20: r[d2] = r[s2] + r[t2]; break;
                            case 0x23: case 0x22: r[d2] = r[s2] - r[t2]; break;
                            case 0x24: r[d2] = r[s2] & r[t2]; break;
                            case 0x25: r[d2] = r[s2] | r[t2]; break;
                            case 0x26: r[d2] = r[s2] ^ r[t2]; break;
                            case 0x27: r[d2] = ~(r[s2] | r[t2]); break;
                            case 0x2A: r[d2] = (s32)r[s2] < (s32)r[t2]; break;
                            case 0x2B: r[d2] = r[s2] < r[t2]; break;
                            case 0x10: r[d2] = hi; break;
                            case 0x12: r[d2] = lo; break;
                            default: port_fatal("[mips] delay slot %08x at %08x", dins, save_pc);
                            }
                            break;
                        case 0x08: case 0x09: r[t2] = r[s2] + i2; break;
                        case 0x0A: r[t2] = (s32)r[s2] < i2; break;
                        case 0x0B: r[t2] = r[s2] < (u32)i2; break;
                        case 0x0C: r[t2] = r[s2] & u2; break;
                        case 0x0D: r[t2] = r[s2] | u2; break;
                        case 0x0E: r[t2] = r[s2] ^ u2; break;
                        case 0x0F: r[t2] = u2 << 16; break;
                        case 0x20: r[t2] = (u32)(s32)*(s8 *)(r[s2] + i2); break;
                        case 0x21: r[t2] = (u32)(s32)*(s16 *)(r[s2] + i2); break;
                        case 0x23: r[t2] = *(u32 *)(r[s2] + i2); break;
                        case 0x24: r[t2] = *(u8 *)(r[s2] + i2); break;
                        case 0x25: r[t2] = *(u16 *)(r[s2] + i2); break;
                        case 0x28: *(u8 *)(r[s2] + i2) = (u8)r[t2]; break;
                        case 0x29: *(u16 *)(r[s2] + i2) = (u16)r[t2]; break;
                        case 0x2B: *(u32 *)(r[s2] + i2) = r[t2]; break;
                        default: port_fatal("[mips] delay slot %08x at %08x", dins, save_pc);
                        }
                        r[0] = 0;
                    }
                    if (f) {
                        u32 *sp = (u32 *)r[29];
                        u64 v = ((NativeFn)f)(r[4], r[5], r[6], r[7], sp[4], sp[5], sp[6], sp[7]);
                        r[2] = (u32)v; r[3] = (u32)(v >> 32);
                    } else {
                        port_stub("<library call from interpreted code>", tgt);
                        r[2] = 0;
                    }
                    pc = cur + 8; npc = pc + 4;
                    continue;
                }
            }
            /* interpreted target (or a plain jump / return): the delay slot executes on the next iteration,
               then control continues at the target */
            pc = save_pc; npc = tgt;
        }
    }
    return r[2] | 0;   /* v0 (v1 is lost for native callers: no game C reads a 64-bit result from here) */
}

/* entry from native code: the call thunk / SIGSEGV path stores the retail address in port_mips_pending and jumps
   here with the caller's return address and arguments on the stack (cdecl) */
u32 port_mips_pending;
u32 port_mips_entry(u32 a0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7) {
    u32 args[8] = {a0, a1, a2, a3, a4, a5, a6, a7};
    u32 pc = port_mips_pending;
    return port_mips_call(pc, args);
}
int port_mips_calls(void) { return calls_interp; }
