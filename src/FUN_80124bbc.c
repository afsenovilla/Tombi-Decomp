// FUNC 80124bbc 340 X000
// MATCHING 80124bbc 340
#include "TOBJ.H"

int FUN_80124bbc(TObj *o, TObj *p)
{
    short dy;
    short e;
    short s;
    unsigned short h;
    unsigned short uy;
    unsigned short sm;
    int lim;
    Fix16 *oh = o->h, *ph = p->h;
    unsigned short u6 = oh->p.whole;
    unsigned short u5 = ph->p.whole;

    { int d;
    uy = u6 - u5;
    d = u5 - (unsigned short)p->box0;
    uy += 6;
    { unsigned short t = d; t += o->box0; uy += t; }
    dy = d;
    }
    lim = 0xe;
    if (uy > dy + (o->box1 + lim))
        return 0;
    sm = o->box2 + (o->y.p.whole - p->y.p.whole);
    e = p->box2 - p->y.p.whole;
    if (sm > e + o->box3)
        return 0;
    s = o->y.p.whole + o->box2 - p->y.p.whole;
    if (s <= 0)
        h = 0;
    else
        h = s * dy / e;
    if (p->h->p.whole - (short)h > o->h->p.whole + o->box0)
        return 0;
    o->h->p.whole = p->h->p.whole - h - o->box0;
    o->h->p.frac = 0;
    return 1;
}
