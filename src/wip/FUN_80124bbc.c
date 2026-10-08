// FUNC 80124bbc 340 X000
// FLAGS -O2 -G0 -fno-schedule-insns
#include "TOBJ.H"
int FUN_80124bbc(TObj *o, TObj *p)
{
    short dy;
    short dx;
    short s;
    short h;
    unsigned short ux;
    unsigned short uy;

    uy = o->h->p.whole - p->h->p.whole + 6;
    dy = p->h->p.whole - p->box0;
    uy += dy + o->box0;
    if (uy > dy + o->box1 + 0xe)
        return 0;
    ux = o->box2 + (o->y.p.whole - p->y.p.whole);
    dx = p->box2 - p->y.p.whole;
    if (ux > dx + o->box3)
        return 0;
    s = o->y.p.whole + o->box2 - p->y.p.whole;
    if (s <= 0)
        h = 0;
    else
        h = s * dy / dx;
    if (p->h->p.whole - h > o->h->p.whole + o->box0)
        return 0;
    o->h->p.whole = p->h->p.whole - h - o->box0;
    o->h->p.frac = 0;
    return 1;
}
