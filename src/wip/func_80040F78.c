/* wip: ncheck score 68 but fewer differing instructions than the old 59 version (38 vs 55 diff lines):
   int r, callees return int, s = a + b computed before the compares (fixes base reg a1 for DAT_1f800278).
   Left: hi in a3 (game a2), r in a2 (game a0), inr() result comes out as xori instead of bnez/move/j/li. */
// FUNC 80040f78 596 MAIN0
extern unsigned short *DAT_1f800278;
extern unsigned short DAT_1f800282, DAT_1f800284;
extern int FUN_8004094c(int, int);
extern int FUN_80040d30(int, int);
extern int func_80040AC0(int, int);
extern int func_80040BFC(int, int);

static __inline__ int inr(short m, short lo, short hi)
{
    if (m < lo) return 0;
    if (lo + hi < m) return 0;
    return 1;
}
int func_80040F78(void *o, int x, int y)
{
    short n, m;
    int r;
    unsigned short w, a, b, c;
    int lo, hi;
    unsigned short s;
    n = *(short *)DAT_1f800278++;
    if (n == 0)
        return 0;
    do {
        w = *DAT_1f800278++;
        n--;
        if ((w & 0x1f) == 0) {
            DAT_1f800278 += 3;
            continue;
        }
        r = 0;
        if (w & 0x10) {
            DAT_1f800284 = (w & 0xe00) >> 9;
            a = *DAT_1f800278++;
            b = *DAT_1f800278++;
            c = *DAT_1f800278++;
            lo = c & 0xf;
            hi = (c >> 4) & 0xf;
            s = a + b;
            if ((short)a < (short)y || (short)y < (short)s)
                r = 0;
            else
                r = inr((short)x % 8, lo, hi);
        } else {
            DAT_1f800284 = (w & 0xe00) >> 9;
            switch (w & 0xf) {
            case 1: r = FUN_8004094c((short)x, (short)y); break;
            case 2: r = FUN_80040d30((short)x, (short)y); break;
            case 4: r = func_80040AC0((short)x, (short)y); break;
            case 8: r = func_80040BFC((short)x, (short)y); break;
            }
        }
        if (r) {
            DAT_1f800282 = w;
            return 1;
        }
    } while (n);
    return 0;
}
