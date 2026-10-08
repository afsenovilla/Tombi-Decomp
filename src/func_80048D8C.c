// FUNC 80048d8c 220 MAIN0
// MATCHING 80048d8c 220
#include "TOBJ.H"
static __inline__ short hit(TObj *o, TObj *e)
{
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 45) > 90)
        return 0;
    if ((unsigned short)((o->h->p.whole - e->h->p.whole) + (e->box0 + (o->box1 - o->box0))) > e->box1 + o->box1)
        return 0;
    if ((unsigned short)((o->y.p.whole - e->y.p.whole) + (e->box2 + (o->box3 - o->box2))) <= o->box3 + e->box3)
        return 1;
    return 0;
}
void func_80048D8C(TObj *o, TObj *e)
{
    if (hit(o, e)) {
        e->b68 = 1;
    }
}
