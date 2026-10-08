// FUNC 800441bc 212 MAIN0
// MATCHING 800441bc 212
#include "TOBJ.H"
#define B(o, n) (((unsigned char *)(o))[n])

static __inline__ short hit(TObj *o, TObj *e)
{
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 45) > 90)
        return 0;
    if ((unsigned short)(o->h->p.whole - e->h->p.whole + (o->box0 + e->box0)) > o->box1 + e->box1)
        return 0;
    if ((unsigned short)(o->y.p.whole - e->y.p.whole + (o->box2 + e->box2)) > o->box3 + e->box3)
        return 0;
    return 1;
}

void func_800441BC(TObj *o, TObj *e)
{
    e->b69 = 0;
    if (hit(o, e)) {
        B(o, 0xa8) = 3;
        B(o, 0xa0) = 1;
        e->b69 = 1;
    }
}
