/* wip: score 65. Falla: el juego copia u/lo/hi a otros registros (move t1,a0; move a1,a0; move v1,t0; move a2,t5),
   parece un inline con parametros; varias formulaciones con inline empeoran (77-91). */
// FUNC 80040ac0 316 MAIN0
extern unsigned short *DAT_1f800278;
unsigned func_80040AC0(int u, short a)
{
    unsigned short x, w, m;
    short sw, sx;
    int lo, hi, k, t2, q2, r;
    x = *DAT_1f800278++;
    w = *DAT_1f800278++;
    m = *DAT_1f800278++;
    lo = m & 0xf;
    hi = (m >> 4) & 0xf;
    sw = w;
    if (sw == 0) return 0;
    sx = x;
    if (sx + 0x10 < a) return 0;
    if (-sw < (int)(unsigned short)(a - x - w - 1)) return 0;
    u &= 7;
    k = u;
    t2 = 0;
    if (u < lo) return 0; if (lo + hi < u) { t2 = u - (lo + hi); k = lo + hi; }
    q2 = hi * (a - sx) / sw;
    return (unsigned)~(((t2 + (k - lo - q2)) << 16) >> 16) >> 31;
}
