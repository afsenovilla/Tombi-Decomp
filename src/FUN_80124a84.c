// FUNC 80124a84 312 X000
// MATCHING 80124a84 312
#include "TOBJ.H"

int FUN_80124a84(TObj *a, TObj *b)
{
    unsigned short u5; int u6; int w;
    short d; short e; short s; short t;
    Fix16 *ah = a->h, *bh = b->h;
    u5 = (unsigned short)b->box0;
    u6 = (unsigned short)ah->p.whole;
    w = u6; w -= u5;
    d = bh->p.whole - u5;
    if ((unsigned short)w <= d) {
        unsigned short sm;
        sm = a->box2 + (a->y.p.whole - b->y.p.whole);
        e = b->box2 - b->y.p.whole;
        if (sm > e + a->box3)
            return 0;
        t = u6 - u5;
        if (t <= 0)
            s = 0;
        else
            s = t * e / d;
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
