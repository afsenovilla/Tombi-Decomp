// FUNC 8004886c 364 MAIN0
// MATCHING 8004886c 364
#include "TOBJ.H"

int FUN_8004886c(TObj *o, TObj *p)
{
    short w;
    short dy;
    short t;
    if ((unsigned short)(o->d->p.whole - p->d->p.whole + 0x2d) >= 0x5b)
        return 0;
    if (o->animFrame & 1)
        w = o->box0;
    else
        w = o->box1 - o->box0;
    if ((unsigned short)(o->h->p.whole - p->h->p.whole + (p->box0 + w)) > p->box1 + o->box1)
        return 0;
    dy = o->y.p.whole - p->y.p.whole;
    t = p->box2 + (o->box3 - o->box2);
    if ((unsigned short)(dy + t) > o->box3 + p->box3)
        return 0;
    if (dy <= 0) {
        if (o->b9c & 1)
            return 0;
        o->y.p.whole = p->y.p.whole - t;
        o->y.p.frac = 0;
        o->b69 = 1;
        return 1;
    }
    if (o->category == 2 && *(unsigned short *)&o->b04 == 0x102)
        return 0;
    o->y.p.whole = p->y.p.whole + (o->box2 + (p->box3 - p->box2));
    return 3;
}
