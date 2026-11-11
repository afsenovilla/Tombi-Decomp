// VERSION BETA_980203 beta980203 80051010 220
#if defined(VERSION_BETA_980203)
// Beta version: SCUS_94236 C of src/func_80052350_scus94236.c (func_80052350_scus94236, twin score 1.000) [-O2 -G0] (tools/demo_sweep.py)
#include "TOBJ.H"

static __inline__ short hit(TObj *a, TObj *b)
{
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return 0;
    if ((unsigned short)(a->h->p.whole - b->h->p.whole + (a->box0 + b->box0)) > a->box1 + b->box1) return 0;
    if ((unsigned short)(a->y.p.whole - b->y.p.whole + (a->box2 + b->box2)) > a->box3 + b->box3) return 0;
    return 1;
}

void func_80052350(TObj *a, TObj *b)
{
    if (hit(a, b)) {
        a->b6a = 1;
        b->active = 4;
        b->b04 = 2;
        b->step = 0;
        b->state = 0;
    }
}
#endif
