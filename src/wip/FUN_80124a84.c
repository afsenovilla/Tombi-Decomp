// FUNC 80124a84 312 X000
#include "TOBJ.H"

static __inline__ int inl(TObj *a, TObj *b)
{
    unsigned short u5, u6;
    short d, e, s;
    u5 = b->box0;
    u6 = a->h->p.whole;
    if ((unsigned short)(u6 - u5) <= (d = b->h->p.whole - u5)) {
        if ((unsigned short)(a->box2 + (a->y.p.whole - b->y.p.whole)) > (e = b->box2 - b->y.p.whole) + a->box3)
            return 0;
        if ((short)(u6 - u5) <= 0)
            s = 0;
        else
            s = (short)(u6 - u5) * e / d;
        if (b->box2 - s <= a->y.p.whole + a->box2) {
            a->y.p.whole = b->box2 - s - a->box2;
            a->y.p.frac = 0;
            a->velY = 0;
            a->b69 = 1;
            return 1;
        }
    }
    return 0;
}

int FUN_80124a84(TObj *a, TObj *b)
{
    return inl(a, b);
}
