// FUNC 80124e34 528 X001
// MATCHING 80124e34 528
#include "TOBJ.H"
#include "raw7.h"

int func_80124E34(TObj *o, TObj *e)
{
    unsigned short dy0;
    short dx, dx2;
    short dy, r;
    int d;
    short v;

    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) > 0x5a) return 0;
    dx = o->h->p.whole - e->h->p.whole;
    if ((unsigned short)(e->box0 + dx) > e->box1) return 0;
    dy0 = o->y.p.whole - e->y.p.whole;
    if ((unsigned short)(dy0 + (o->box2 + e->box2)) > o->box3 + e->box3) return 0;
    if ((short)dy0 < 0x19) {
        dx2 = e->d30 - e->h->p.whole;
        dy = e->d34 - e->y.p.whole;
        if (dx <= 0) r = 0;
        else r = dx * dy / dx2;
        if (e->y.p.whole + r > o->y.p.whole + o->box2) return 0;
        o->y.p.whole = e->y.p.whole + r - o->box2;
        o->y.p.frac = 0;
        o->velY = 0;
        o->b69 = 1;
        if (U8(o, 0xac) != 3) e->b69 = 1;
        d = e->d8c;
        if (d > 0x800) {
            v = ((unsigned)(-d) & 0xfff) >> 6;
            if (v >= 9) v = 8;
        } else {
            v = (d >> 6) & 0xff;
            if (v >= 9) v = 8;
            v = -v;
        }
        o->wb0 = v;
        return 1;
    }
    o->y.p.whole = e->y.p.whole + ((e->box3 - e->box2) + (o->box3 - o->box2));
    if (o->velY < 0) o->velY = 0;
}
