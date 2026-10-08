// FUNC 80124a84 312 X000
// wip score 12 (was 27): types brute-forced (u5 short, t int). Left: game loads both ->h (lw 0x40) before lhu box0, recomputes u6-u5 (subu t2,t1) for t, and orders the e computation (subu before andi) differently.
#include "TOBJ.H"

int FUN_80124a84(TObj *a, TObj *b)
{
    short u5; int u6;
    short d; short e; short s; int t;
    u5 = (unsigned short)b->box0;
    u6 = (unsigned short)a->h->p.whole;
    if ((unsigned short)(u6 - u5) <= (d = b->h->p.whole - u5)) {
        if ((unsigned short)(a->box2 + (a->y.p.whole - b->y.p.whole)) > (e = b->box2 - b->y.p.whole) + a->box3)
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
