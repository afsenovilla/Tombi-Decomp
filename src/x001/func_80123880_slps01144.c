// VERSION SLPS_01144 x01 80123880 512
// FLAGS -O2 -G0 -fno-strength-reduce
// JP-only: pushObjectOutOfObjectBox variant for a sloped surface (a->y follows the b->d30/d34 slope while a is within 24 of the top)
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
int func_80123880_slps01144(TObj *a, TObj *b)
{
    int k, x;
    short s;
    short dx, dy, sl;
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return 0;
    dx = (unsigned short)a->h->p.whole - (unsigned short)b->h->p.whole;
    if ((unsigned short)(b->box0 + dx) > b->box1) return 0;
    dy = (unsigned short)a->y.p.whole - (unsigned short)b->y.p.whole;
    if ((unsigned short)(dy + ((unsigned short)a->box2 + (unsigned short)b->box2)) > a->box3 + b->box3) return 0;
    if (dy < 25) {
        short r = b->d30 - (unsigned short)b->h->p.whole;
        short u = b->d34 - (unsigned short)b->y.p.whole;
        if (dx <= 0) sl = 0;
        else sl = dx * u / r;
        if (b->y.p.whole + sl > a->y.p.whole + a->box2) return 0;
        a->y.p.whole = b->y.p.whole + sl - a->box2;
        a->y.p.frac = 0;
        a->velY = 0;
        a->b69 = 1;
        k = b->d8c;
        b->b69 = 1;
        if (k > 2048) {
            x = (unsigned)(-k & 0xfff) >> 6;
            s = x;
            if (x > 8) s = 8;
        } else {
            x = (unsigned char)(k >> 6);
            s = x;
            if (x > 8) s = 8;
            s = -s;
        }
        a->wb0 = s;
        return 1;
    }
    a->y.p.whole = (unsigned short)b->y.p.whole + ((b->box3 - b->box2) + (a->box3 - a->box2));
    if (a->velY < 0) a->velY = 0;
}
#endif
