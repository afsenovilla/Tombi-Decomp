// FUNC 80047e74 200 MAIN0
// MATCHING 80047e74 200
#include "TOBJ.H"

static __inline__ int hit(TObj *a, TObj *b)
{
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return 0;
    if ((unsigned short)(a->h->p.whole - b->h->p.whole + (b->box0 + (a->box1 - a->box0))) > b->box1 + a->box1) return 0;
    return (unsigned short)(a->y.p.whole - b->y.p.whole + (b->box2 + (a->box3 - a->box2))) <= a->box3 + b->box3;
}

int func_80047E74(TObj *a, TObj *b)
{
    return hit(a, b);
}
