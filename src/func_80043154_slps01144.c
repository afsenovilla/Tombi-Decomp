// VERSION SLPS_01144 slps01144 80043154 292
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
extern unsigned short *D_1F800278;
extern short D_1F80027E, D_1F800280, D_1F800284;
extern unsigned short D_1F800282;
#include "TOBJ.H"
extern short *getCollisionPlaneAt(int a, int b);
#define SEG D_1F800278
int func_80043154(TObj *o)
{
    short n;
    register unsigned short w asm("$5");
    register unsigned short h asm("$4");
    unsigned short x0, y;
    register int k asm("$2");
    short *q;
    q = getCollisionPlaneAt(o->h->p.whole, o->d->p.whole);
    SEG = (unsigned short *)(q + 1);
    n = *q;
    if (n == 0) return 0;
    y = o->y.p.whole;
    do {
        n--;
        w = *SEG++;
        x0 = *SEG++;
        h = *SEG++;
        SEG++;
        if ((short)(x0 + h - y) <= 0) {
            D_1F800284 = (w & 0xe00) >> 9;
            asm("" ::: "memory");
            k = (w >> 5) & 0xf;
            D_1F80027E = h;
            D_1F800280 = x0;
            D_1F800282 = w;
            if (k == 13) {
                *((unsigned char *)o + 0xa1) = 2;
                return 1;
            }
            return 0;
        }
    } while (n != 0);
    return 0;
}
#endif
