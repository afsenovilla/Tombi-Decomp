// FUNC 8004874c 288 MAIN0
// MATCHING 8004874c 288
#include "TOBJ.H"

int FUN_8004874c(TObj *o, TObj *e)
{
    short dx, w, r;
    char pad;
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 45) > 90)
        return 0;
    dx = o->h->p.whole - e->h->p.whole;
    w = e->box0 + (o->box1 - o->box0);
    if ((unsigned short)(dx + w) > e->box1 + o->box1)
        return 0;
    if ((unsigned short)(o->y.p.whole - e->y.p.whole + (e->box2 + (o->box3 - o->box2))) > o->box3 + e->box3)
        return 0;
    if (dx < 0)
        r = -w;
    else
        r = o->box0 + (e->box1 - e->box0);
    o->h->p.whole = e->h->p.whole + r;
    o->b9d = r < 0 ? 2 : 3;
    return 2;
}
