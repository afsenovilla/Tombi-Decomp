/* Software GTE (COP2): the register file and the commands the game uses (RTPS, RTPT, NCLIP, AVSZ3, AVSZ4, NCCS,
 * NCCT, MVMVA for the library helpers).  The gte_* macros of the game sources are translated by
 * port/tools/gen_port.py into port_gte_mtc2/mfc2/ctc2/cfc2/op calls on this file.
 * Precision follows the documented hardware behaviour (psx-spx "GTE"), with the UNR divider replaced by an
 * exact rounded division; good enough for gameplay, refine against an emulator for pixel-exact output. */
#include "port_runtime.h"

typedef int s32; typedef unsigned int u32; typedef short s16; typedef unsigned short u16; typedef long long s64;

static s32 D[32];   /* data registers */
static s32 C[32];   /* control registers */
static u32 FLAG;

#define VX(n) ((s16)(D[(n) * 2] & 0xFFFF))
#define VY(n) ((s16)((u32)D[(n) * 2] >> 16))
#define VZ(n) ((s16)(D[(n) * 2 + 1] & 0xFFFF))
#define IR0 D[8]
#define IR1 D[9]
#define IR2 D[10]
#define IR3 D[11]
#define MAC0 D[24]
#define MAC1 D[25]
#define MAC2 D[26]
#define MAC3 D[27]
#define M16LO(r) ((s16)(C[r] & 0xFFFF))
#define M16HI(r) ((s16)((u32)C[r] >> 16))

static void mat(int base, s16 m[3][3]) {
    m[0][0] = M16LO(base); m[0][1] = M16HI(base); m[0][2] = M16LO(base + 1);
    m[1][0] = M16HI(base + 1); m[1][1] = M16LO(base + 2); m[1][2] = M16HI(base + 2);
    m[2][0] = M16LO(base + 3); m[2][1] = M16HI(base + 3); m[2][2] = M16LO(base + 4);
}

static s32 clampf(s64 v, s32 lo, s32 hi, u32 flag) {
    if (v < lo) { FLAG |= flag; return lo; }
    if (v > hi) { FLAG |= flag; return hi; }
    return (s32)v;
}
static void set_ir(int lm) {
    s32 lo = lm ? 0 : -0x8000;
    IR1 = clampf(MAC1, lo, 0x7FFF, 1u << 24);
    IR2 = clampf(MAC2, lo, 0x7FFF, 1u << 23);
    IR3 = clampf(MAC3, lo, 0x7FFF, 1u << 22);
}

static void rtp(int n, int last) {
    s16 r[3][3]; mat(0, r);
    s64 x = VX(n), y = VY(n), z = VZ(n);
    s64 m1 = ((s64)C[5] << 12) + r[0][0] * x + r[0][1] * y + r[0][2] * z;
    s64 m2 = ((s64)C[6] << 12) + r[1][0] * x + r[1][1] * y + r[1][2] * z;
    s64 m3 = ((s64)C[7] << 12) + r[2][0] * x + r[2][1] * y + r[2][2] * z;
    MAC1 = (s32)(m1 >> 12); MAC2 = (s32)(m2 >> 12); MAC3 = (s32)(m3 >> 12);
    set_ir(0);
    /* SZ FIFO */
    D[16] = D[17]; D[17] = D[18]; D[18] = D[19];
    D[19] = clampf(m3 >> 12, 0, 0xFFFF, 1u << 18);
    u32 h = (u16)C[26], sz = (u32)D[19];
    u32 q;
    if (sz * 2 <= h) { q = 0x1FFFF; FLAG |= 1u << 17; }
    else { q = (u32)((((s64)h << 17) / sz + 1) >> 1); if (q > 0x1FFFF) { q = 0x1FFFF; FLAG |= 1u << 17; } }
    s64 sx = ((s64)C[24] + (s64)IR1 * q) >> 16;
    s64 sy = ((s64)C[25] + (s64)IR2 * q) >> 16;
    D[12] = D[13]; D[13] = D[14];
    s32 SX = clampf(sx, -0x400, 0x3FF, 1u << 14), SY = clampf(sy, -0x400, 0x3FF, 1u << 13);
    D[14] = (SX & 0xFFFF) | ((u32)SY << 16);
    D[15] = D[14];
    if (last) {
        s64 m0 = (s64)(s16)C[27] * q + C[28];
        MAC0 = (s32)m0;
        IR0 = clampf(m0 >> 12, 0, 0x1000, 1u << 12);
    }
}

static void ncc(int n) {
    s16 l[3][3], c[3][3]; mat(8, l); mat(16, c);
    s64 x = VX(n), y = VY(n), z = VZ(n);
    MAC1 = (s32)((l[0][0] * x + l[0][1] * y + l[0][2] * z) >> 12);
    MAC2 = (s32)((l[1][0] * x + l[1][1] * y + l[1][2] * z) >> 12);
    MAC3 = (s32)((l[2][0] * x + l[2][1] * y + l[2][2] * z) >> 12);
    set_ir(1);
    s64 a = IR1, b = IR2, d = IR3;
    MAC1 = (s32)((((s64)C[13] << 12) + c[0][0] * a + c[0][1] * b + c[0][2] * d) >> 12);
    MAC2 = (s32)((((s64)C[14] << 12) + c[1][0] * a + c[1][1] * b + c[1][2] * d) >> 12);
    MAC3 = (s32)((((s64)C[15] << 12) + c[2][0] * a + c[2][1] * b + c[2][2] * d) >> 12);
    set_ir(1);
    u32 rgbc = (u32)D[6];
    MAC1 = (s32)((((s64)(rgbc & 0xFF) * IR1) << 4) >> 12);
    MAC2 = (s32)((((s64)((rgbc >> 8) & 0xFF) * IR2) << 4) >> 12);
    MAC3 = (s32)((((s64)((rgbc >> 16) & 0xFF) * IR3) << 4) >> 12);
    set_ir(1);
    u32 R = clampf(MAC1 >> 4, 0, 255, 1u << 21), G = clampf(MAC2 >> 4, 0, 255, 1u << 20), B = clampf(MAC3 >> 4, 0, 255, 1u << 19);
    D[20] = D[21]; D[21] = D[22];
    D[22] = R | (G << 8) | (B << 16) | (rgbc & 0xFF000000u);
}

static void mvmva(u32 op) {
    int sf = (op >> 19) & 1, mx = (op >> 17) & 3, v = (op >> 15) & 3, cv = (op >> 13) & 3, lm = (op >> 10) & 1;
    s16 m[3][3]; mat(mx == 0 ? 0 : mx == 1 ? 8 : 16, m);
    s64 x, y, z;
    if (v == 3) { x = (s16)IR1; y = (s16)IR2; z = (s16)IR3; } else { x = VX(v); y = VY(v); z = VZ(v); }
    s64 t1 = 0, t2 = 0, t3 = 0;
    if (cv == 0) { t1 = C[5]; t2 = C[6]; t3 = C[7]; } else if (cv == 1) { t1 = C[13]; t2 = C[14]; t3 = C[15]; }
    else if (cv == 2) { t1 = C[21]; t2 = C[22]; t3 = C[23]; }
    int sh = sf ? 12 : 0;
    MAC1 = (s32)(((t1 << 12) + m[0][0] * x + m[0][1] * y + m[0][2] * z) >> sh);
    MAC2 = (s32)(((t2 << 12) + m[1][0] * x + m[1][1] * y + m[1][2] * z) >> sh);
    MAC3 = (s32)(((t3 << 12) + m[2][0] * x + m[2][1] * y + m[2][2] * z) >> sh);
    set_ir(lm);
}

void port_gte_op(unsigned long op) {
    FLAG = 0;
    switch (op & 0x3F) {
    case 0x01: rtp(0, 1); break;                                     /* RTPS */
    case 0x30: rtp(0, 0); rtp(1, 0); rtp(2, 1); break;               /* RTPT */
    case 0x06: {                                                     /* NCLIP */
        s64 x0 = (s16)D[12], y0 = (s16)(D[12] >> 16), x1 = (s16)D[13], y1 = (s16)(D[13] >> 16);
        s64 x2 = (s16)D[14], y2 = (s16)(D[14] >> 16);
        MAC0 = (s32)(x0 * y1 + x1 * y2 + x2 * y0 - x0 * y2 - x1 * y0 - x2 * y1);
        break;
    }
    case 0x2D: {                                                     /* AVSZ3 */
        s64 m = (s64)(s16)C[29] * ((u16)D[17] + (u16)D[18] + (u16)D[19]);
        MAC0 = (s32)m; D[7] = clampf(m >> 12, 0, 0xFFFF, 1u << 18);
        break;
    }
    case 0x2E: {                                                     /* AVSZ4 */
        s64 m = (s64)(s16)C[30] * ((u16)D[16] + (u16)D[17] + (u16)D[18] + (u16)D[19]);
        MAC0 = (s32)m; D[7] = clampf(m >> 12, 0, 0xFFFF, 1u << 18);
        break;
    }
    case 0x1B: ncc(0); break;                                        /* NCCS */
    case 0x3F: ncc(0); ncc(1); ncc(2); break;                        /* NCCT */
    case 0x12: mvmva((u32)op); break;                                /* MVMVA */
    default: port_printf("[port] GTE op %08lx not implemented\n", op);
    }
    if (FLAG & 0x7F87E000u) FLAG |= 0x80000000u;
}

void port_gte_mtc2(int r, unsigned long v) {
    switch (r) {
    case 15: D[12] = D[13]; D[13] = D[14]; D[14] = (s32)v; D[15] = (s32)v; break;  /* SXYP pushes */
    case 7: case 16: case 17: case 18: case 19: D[r] = (s32)(v & 0xFFFF); break;
    case 8: case 9: case 10: case 11: D[r] = (s16)v; break;
    case 28: D[28] = (s32)(v & 0x7FFF);
        IR1 = (v & 0x1F) << 7; IR2 = ((v >> 5) & 0x1F) << 7; IR3 = ((v >> 10) & 0x1F) << 7; break;
    case 30: { D[30] = (s32)v; s32 x = (s32)v; u32 u = x < 0 ? ~(u32)x : (u32)x; int n = 0;
        while (n < 32 && !(u & 0x80000000u)) { u <<= 1; n++; } D[31] = n; break; }
    case 29: case 31: break;
    default: D[r] = (s32)v;
    }
}

unsigned long port_gte_mfc2(int r) {
    switch (r) {
    case 1: case 3: case 5: case 8: case 9: case 10: case 11: return (unsigned long)(s32)(s16)D[r];
    case 7: case 16: case 17: case 18: case 19: return (u16)D[r];
    case 15: return (u32)D[14];
    case 28: case 29: {
        s32 r5 = IR1 >> 7, g5 = IR2 >> 7, b5 = IR3 >> 7;
        r5 = r5 < 0 ? 0 : r5 > 31 ? 31 : r5; g5 = g5 < 0 ? 0 : g5 > 31 ? 31 : g5; b5 = b5 < 0 ? 0 : b5 > 31 ? 31 : b5;
        return (u32)(r5 | (g5 << 5) | (b5 << 10));
    }
    default: return (u32)D[r];
    }
}

void port_gte_ctc2(int r, unsigned long v) {
    if (r == 31) { FLAG = v & 0x7FFFF000u; if (FLAG & 0x7F87E000u) FLAG |= 0x80000000u; return; }
    if (r == 4 || r == 12 || r == 20 || r == 26 || r == 27 || r == 29 || r == 30) v = (unsigned long)(s32)(s16)v;
    if (r == 26) v &= 0xFFFF;
    C[r] = (s32)v;
}

unsigned long port_gte_cfc2(int r) { return r == 31 ? FLAG : (u32)C[r]; }

void port_gte_unsupported(const char *ins) { port_fatal("GTE macro instruction not translated: %s", ins); }
