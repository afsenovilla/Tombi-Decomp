// FUNC 80040ac0 316 MAIN0
/* score 49 (ncheck; b20, was 63). Game: after u &= 7 it copies u (t1 = k, a1), lo (v1) and hi (a2) and does the two range compares on the copies, while the arithmetic (lo + hi, u - (lo + hi), k - lo) uses the originals: looks like an inline doing only the compares with outer code doing the math. Here: inline with narrow param types (gives andi instead of plain moves). Also (short)a is computed before (short)x in the game. Tried: r=-1/0/1 compare inline (89), A/X short locals, compare forms. */
extern unsigned short *DAT_1f800278;

static __inline__ int calc(unsigned char u, int lo, unsigned short hi, int dy, short sw)
{
    int t = 0;
    u &= 7;
    if (u < lo) return 0;
    if (lo + hi < u) {
        t = u - (lo + hi);
        u = lo + hi;
    }
    return (short)(t + (u - lo - hi * dy / sw)) >= 0;
}

unsigned func_80040AC0(int u, int a)
{
    unsigned short x;
    unsigned short w;
    unsigned short m;
    int lo;
    int hi;
    short sw;
    x = *DAT_1f800278++;
    w = *DAT_1f800278++;
    m = *DAT_1f800278++;
    lo = m & 0xf;
    hi = (m >> 4) & 0xf;
    sw = w;
    if (sw == 0) return 0;
    if ((short)x + 0x10 < (short)a) return 0;
    if (-sw < (unsigned short)(a - x - w - 1)) return 0;
    return calc(u, lo, hi, (short)a - (short)x, sw);
}
