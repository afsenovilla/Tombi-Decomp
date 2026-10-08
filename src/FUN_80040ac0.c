// FUNC 80040ac0 316 MAIN0
extern unsigned short *DAT_1f800278;

unsigned FUN_80040ac0(int u, short a)
{
    unsigned short *q;
    short x, sw;
    int lo, hi, m, k;
    int t2, q2;
    q = DAT_1f800278;
    x = *q;
    DAT_1f800278 = q + 1;
    DAT_1f800278 = q + 2;
    sw = q[1];
    DAT_1f800278 = q + 3;
    m = q[2];
    lo = m & 0xf;
    hi = (m >> 4) & 0xf;
    if (sw != 0) {
        if (x + 0x10 < a) return 0;
        if (-sw < (int)(unsigned short)(a - x - sw - 1)) return 0;
        u &= 7;
        k = u;
        t2 = 0;
        if (u < lo) return 0;
        if (lo + hi < u) {
            t2 = u - (lo + hi);
            k = lo + hi;
        }
        q2 = hi * (a - x) / sw;
        return (unsigned)~(((t2 + (k - lo - q2)) << 16) >> 16) >> 31;
    }
    return 0;
}
