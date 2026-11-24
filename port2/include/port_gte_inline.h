/* The 3DS (include/port_prelude.h): the GTE in the game's code without calls where it can be.  The register moves
   in line (the game makes ~7700 of them a frame, each a call into runtime/gte.c: with the register number a constant,
   one of these is a store or a load), and a command, a constant in the game's code too, straight to its own function
   (runtime/gte.c port_gte_rtps ...) or, the short ones (NCLIP, AVSZ3, AVSZ4), in line: no call, no switch.  The same
   results as runtime/gte.c's port_gte_op (port2/tools/gte_arm_test.c). */
#ifndef PORT_GTE_INLINE_H
#define PORT_GTE_INLINE_H
extern int port_gte_D[32], port_gte_C[32];
extern unsigned int port_gte_FLAG;
void port_gte_op(unsigned long op);
void port_gte_rtps(void);
void port_gte_rtpt(void);
void port_gte_nccs(void);
void port_gte_ncct(void);
static inline __attribute__((always_inline)) void port_gte_mtc2_i(int r, unsigned long v) {
    int *gd = port_gte_D;
    switch (r) {
    case 15: gd[12] = gd[13]; gd[13] = gd[14]; gd[14] = (int)v; gd[15] = (int)v; break;
    case 7: case 16: case 17: case 18: case 19: gd[r] = (int)(v & 0xFFFF); break;
    case 8: case 9: case 10: case 11: gd[r] = (short)v; break;
    case 28: gd[28] = (int)(v & 0x7FFF);
        gd[9] = (v & 0x1F) << 7; gd[10] = ((v >> 5) & 0x1F) << 7; gd[11] = ((v >> 10) & 0x1F) << 7; break;
    case 30: { gd[30] = (int)v; int x = (int)v; unsigned u = x < 0 ? ~(unsigned)x : (unsigned)x; int n = 0;
        while (n < 32 && !(u & 0x80000000u)) { u <<= 1; n++; } gd[31] = n; break; }
    case 29: case 31: break;
    default: gd[r] = (int)v;
    }
}
static inline __attribute__((always_inline)) unsigned long port_gte_mfc2_i(int r) {
    int *gd = port_gte_D;
    switch (r) {
    case 1: case 3: case 5: case 8: case 9: case 10: case 11: return (unsigned long)(int)(short)gd[r];
    case 7: case 16: case 17: case 18: case 19: return (unsigned short)gd[r];
    case 15: return (unsigned)gd[14];
    case 28: case 29: {
        int r5 = gd[9] >> 7, g5 = gd[10] >> 7, b5 = gd[11] >> 7;
        r5 = r5 < 0 ? 0 : r5 > 31 ? 31 : r5; g5 = g5 < 0 ? 0 : g5 > 31 ? 31 : g5; b5 = b5 < 0 ? 0 : b5 > 31 ? 31 : b5;
        return (unsigned)(r5 | (g5 << 5) | (b5 << 10));
    }
    default: return (unsigned)gd[r];
    }
}
static inline __attribute__((always_inline)) void port_gte_ctc2_i(int r, unsigned long v) {
    if (r == 31) { port_gte_FLAG = v & 0x7FFFF000u; if (port_gte_FLAG & 0x7F87E000u) port_gte_FLAG |= 0x80000000u; return; }
    if (r == 4 || r == 12 || r == 20 || r == 26 || r == 27 || r == 29 || r == 30) v = (unsigned long)(int)(short)v;
    if (r == 26) v &= 0xFFFF;
    port_gte_C[r] = (int)v;
}
static inline __attribute__((always_inline)) unsigned long port_gte_cfc2_i(int r) { return r == 31 ? port_gte_FLAG : (unsigned)port_gte_C[r]; }
/* AVSZ3 / AVSZ4: MAC0 = ZSF * the sum of the last SZ values, OTZ = that / 4096 limited to 0 .. 0xFFFF (flag bit 18) */
static inline __attribute__((always_inline)) void port_gte_avsz_i(int zsf, unsigned sum) {
    int *gd = port_gte_D;
    long long m = (long long)(short)port_gte_C[zsf] * sum, o = m >> 12;
    gd[24] = (int)m;
    if (o < 0) { gd[7] = 0; port_gte_FLAG = (1u << 18) | 0x80000000u; }
    else if (o > 0xFFFF) { gd[7] = 0xFFFF; port_gte_FLAG = (1u << 18) | 0x80000000u; }
    else { gd[7] = (int)o; port_gte_FLAG = 0; }
}
static inline __attribute__((always_inline)) void port_gte_op_i(unsigned long op) {
    int *gd = port_gte_D;
    switch (op & 0x3F) {
    case 0x01: port_gte_rtps(); return;
    case 0x30: port_gte_rtpt(); return;
    case 0x1B: port_gte_nccs(); return;
    case 0x3F: port_gte_ncct(); return;
    case 0x06: {   /* NCLIP (modulo 2^32, as runtime/gte.c) */
        int x0 = (short)gd[12], y0 = (short)(gd[12] >> 16), x1 = (short)gd[13], y1 = (short)(gd[13] >> 16);
        int x2 = (short)gd[14], y2 = (short)(gd[14] >> 16);
        gd[24] = (int)((unsigned)(x0 * y1) + (unsigned)(x1 * y2) + (unsigned)(x2 * y0) - (unsigned)(x0 * y2) - (unsigned)(x1 * y0) - (unsigned)(x2 * y1));
        port_gte_FLAG = 0;
        return;
    }
    case 0x2D: port_gte_avsz_i(29, (unsigned)(unsigned short)gd[17] + (unsigned short)gd[18] + (unsigned short)gd[19]); return;
    case 0x2E: port_gte_avsz_i(30, (unsigned)(unsigned short)gd[16] + (unsigned short)gd[17] + (unsigned short)gd[18] + (unsigned short)gd[19]); return;
    default: port_gte_op(op);
    }
}
#endif
