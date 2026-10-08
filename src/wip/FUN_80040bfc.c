// FUNC 80040bfc 308 MAIN0
/* score 60 (was 65, greedy param types). Game keeps inline-param copies (move $9,$4 k=u; move $6,$4; move $5,lo; move $3,hi) used by the lo+hi<u and u<lo tests and the mult, while t2=lo-u and k-lo use the originals; early returns jump to the final jr with v0=0. Probably nested inlines; not found yet. */
extern unsigned short *DAT_1f800278;

static __inline__ int chk(int u, int lo, short hi, short s, unsigned short d, int sw)
{
    short k = u;
    int t2 = 0;
    if (lo + hi < u) return 0;
    if (u < lo) {
        t2 = lo - u;
        k = s;
    }
    return (short)(k - lo - hi * d / sw - t2) <= 0;
}

int FUN_80040bfc(int u, short a)
{
    unsigned short x, w, m;
    short sw, sx;
    int lo, hi, s;
    x = *DAT_1f800278++;
    w = *DAT_1f800278++;
    m = *DAT_1f800278++;
    lo = m & 0xf;
    hi = (m >> 4) & 0xf;
    sw = w;
    s = lo;
    if (sw != 0) {
        sx = x;
        if (sx + 0x10 < a) return 0;
        if (-sw < (int)(unsigned short)(a - x - w - 1)) return 0;
        return chk(u & 7, lo, hi, s, a - sx, sw);
    }
    return 0;
}
