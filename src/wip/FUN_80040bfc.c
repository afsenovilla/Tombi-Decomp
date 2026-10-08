// FUNC 80040bfc 308 MAIN0
/* score 4 (b34, was 60): layout fixed with `if (sw == 0) goto ret0; ... if (lo2 + hi2 < u2) { ret0: return 0; }` and
   explicit short copies u2/lo2/hi2 (the label keeps them alive). Left: (1) game emits `move a2,a0` (u2) right after
   k = u, ours schedules it after the addu; (2) `t2 = lo - u` uses the original lo (t0) in the game, ours gets lo2 (cse).
   Tried: all statement orders of k/u2/lo2/hi2/t2, type product of lo/lo2/u2/k, t2 = s - u / lo - (u & 7). */
extern unsigned short *DAT_1f800278;

int FUN_80040bfc(int u, int a)
{
    unsigned short x, w, m;
    short sw;
    int sx, sa;
    short lo, hi; short s;
    short k; int t2; short u2, lo2, hi2;
    x = *DAT_1f800278++;
    w = *DAT_1f800278++;
    m = *DAT_1f800278++;
    lo = m & 0xf;
    hi = (m >> 4) & 0xf;
    sw = w;
    s = lo;
    if (sw == 0) goto ret0;
    sa = (short)a;
    sx = (short)x;
    if (sx + 0x10 < sa) return 0;
    if ((unsigned short)(a - x - w - 1) > -sw) return 0;
    u &= 7;
    k = u;
    u2 = u;
    lo2 = lo;
    hi2 = hi;
    t2 = 0;
    if (lo2 + hi2 < u2) {
ret0:
        return 0;
    }
    if (u2 < lo2) {
        t2 = lo - u;
        k = s;
    }
    return (short)(k - lo - hi2 * (sa - sx) / sw - t2) <= 0;
}
