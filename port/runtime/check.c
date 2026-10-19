/* --check ADDR: differential test of a hand translation.  Every call of the retail function at ADDR runs twice
 * on the same machine state: first the retail code in the MIPS interpreter, then the native translation.  The PS1
 * RAM and the scratchpad written by each run, and the return values, are compared; the native result is kept.
 * The interpreter calls native callees the same way in both runs, so a difference is the translation's. */
#include "port_runtime.h"

typedef unsigned int u32; typedef unsigned char u8; typedef unsigned long long u64;

u32 port_check_addr;
static u8 before[PSX_RAM_SIZE], after_interp[PSX_RAM_SIZE];
static u8 spad_before[0x400], spad_interp[0x400];
static int calls, bad;

static void copy(void *d, const void *s, unsigned n) {
    u32 *a = d; const u32 *b = s;
    for (unsigned i = 0; i < n / 4; i++) a[i] = b[i];
}
static int first_diff(const u8 *a, const u8 *b, unsigned n) {
    for (unsigned i = 0; i < n; i++) if (a[i] != b[i]) return (int)i;
    return -1;
}

typedef u64 (*Fn8)(u32, u32, u32, u32, u32, u32, u32, u32);
static u64 check_entry(u32 a0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6, u32 a7) {
    u32 args[8] = { a0, a1, a2, a3, a4, a5, a6, a7 };
    extern u32 port_mips_call(u32 entry, const u32 *args);
    u8 *ram = (u8 *)PSX_RAM_BASE, *spad = (u8 *)PSX_SPAD_BASE;
    u32 addr = port_check_addr;
    port_check_addr = 0;                         /* the native lookup below and nested calls: no check */
    Fn8 nat = (Fn8)port_lookup(addr);
    copy(before, ram, PSX_RAM_SIZE); copy(spad_before, spad, 0x400);
    u32 ri = port_mips_call(addr, args);
    copy(after_interp, ram, PSX_RAM_SIZE); copy(spad_interp, spad, 0x400);
    copy(ram, before, PSX_RAM_SIZE); copy(spad, spad_before, 0x400);
    u64 rn = nat ? nat(a0, a1, a2, a3, a4, a5, a6, a7) : 0;
    calls++;
    int dr = first_diff(after_interp, ram, PSX_RAM_SIZE), ds = first_diff(spad_interp, spad, 0x400);
    if (!nat || dr >= 0 || ds >= 0 || (u32)rn != ri) {
        if (bad++ < 20) {
            port_printf("[check] %08x call %d (frame %d) args %08x %08x %08x %08x: ret interp %08x native %08x",
                        addr, calls, port_frame, a0, a1, a2, a3, ri, (u32)rn);
            if (dr >= 0) port_printf("; RAM %08x interp %02x native %02x", PSX_RAM_BASE + dr, after_interp[dr], ram[dr]);
            if (ds >= 0) port_printf("; spad %08x interp %02x native %02x", PSX_SPAD_BASE + ds, spad_interp[ds], spad[ds]);
            port_printf("%s\n", nat ? "" : " (no native code)");
        }
    }
    port_check_addr = addr;
    return rn;
}
void *port_check_hook(u32 a) { return a == port_check_addr && a ? (void *)check_entry : 0; }
void port_check_report(void) {
    if (port_check_addr) port_printf("[check] %08x: %d calls compared, %d differences\n", port_check_addr, calls, bad);
}
