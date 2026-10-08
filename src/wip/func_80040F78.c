/* wip: score 59 (ncheck). Inline inr() con lo/hi unsigned short y r short mejora de 112 a 59.
   Falla: registro del puntero DAT_1f800278 (juego a1, nuestro v1), la copia lo->v1 del inline y el orden a+b. */
// FUNC 80040f78 596 MAIN0
extern unsigned short *DAT_1f800278;
extern unsigned short DAT_1f800282, DAT_1f800284;
extern short FUN_8004094c(int, int);
extern short FUN_80040d30(int, int);
extern short func_80040AC0(int, int);
extern short func_80040BFC(int, int);

static __inline__ short inr(short m, unsigned short lo, unsigned short hi)
{
    if (m < lo) return 0;
    if (lo + hi < m) return 0;
    return 1;
}
int func_80040F78(void *o, int x, int y)
{
    short n, m;
    short r;
    unsigned short w, a, b, c;
    int lo, hi;
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
            if ((short)a < (short)y || (short)y < (short)(a + b))
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
