// VERSION SLPS_01144 x10 8011F240 576
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "gv2.h"
#include "TOBJ.H"
extern int SquareRoot0(int);
extern int ratan2(int, int);
extern int rsin(int);
extern int rcos(int);

void func_8011F240_slps01144(TObj *o, TObj *e)
{
    unsigned short dx, dy;
    short y;
    int x;
    int a;
    short sa;
    int one;
    unsigned short t0;

    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) < 0x5b) {
        { unsigned short t1 = o->h->p.whole, t2 = e->h->p.whole; register unsigned short t3 asm("$2") = e->box0; dx = t1 - t2; t3 += dx; t0 = t3; }
        if (e->box1 >= t0) {
            { unsigned short t1 = o->y.p.whole, t2 = e->y.p.whole; register unsigned short t3 asm("$2") = e->box2; dy = t1 - t2; t3 += dy; t0 = t3; }
            if (e->box3 >= t0) {
                x = (short)dx;
                y = dy;
                if (SquareRoot0(x * x + y * y) < (short)e->box0) {
                    x = ratan2(y, x);
                    sa = x;
                    o->y.raw = e->y.raw + ((rsin(sa) * (short)e->box0) << 4);
                    o->h->raw = e->h->raw + ((rcos(sa) * (short)e->box0) << 4);
    one = 1;
    e->b69 = one;
    e->velX = dx;
    e->velY = dy;
    if (((unsigned char*)&GV2.bc3)[0] & 0x40) {
        if (1 & o->b9c) return;
        if ((unsigned short)((x & 0xfff) - 0x901) < 0x5ff) goto push;
        if (o->velY < 0) o->velY = 0;
    } else {
        if (o->b9c & 1) return;
        if (o->y.p.whole >= -0x3bb) return;
        if ((unsigned short)((x & 0xfff) - 0x901) >= 0x5ff) return;
    push:
        o->h->raw += e->velH << 7;
        o->wb0 = 0;
        o->y.p.frac = 0;
        o->b69 = one;
        o->velY = 0;
    }
                }
            }
        }
    }
}
#endif
