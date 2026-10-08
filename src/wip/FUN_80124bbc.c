// FUNC 80124bbc 340 X000
// FLAGS -O2 -G0 -fno-schedule-insns
/* score 40 (was 58): dx before uy, box3 + dx order, h unsigned short. Left: 2nd test load order (game loads p->box2, o->box3 early, (o->y-p->y) in a0), box0/p->h->whole regs swapped in the 3rd test, tail li v0,1 scheduled before sh frac (ours: in j slot). */
#include "TOBJ.H"
int FUN_80124bbc(TObj *o, TObj *p)
{
    short dy;
    short dx;
    short s;
    unsigned short h;
    unsigned short uy;
    int lim;

    uy = o->h->p.whole - p->h->p.whole + 6;
    dy = p->h->p.whole - p->box0;
    uy += dy + o->box0;
    lim = o->box1 + 0xe;
    if (uy > dy + lim)
        return 0;
    dx = p->box2 - p->y.p.whole;
    uy = o->box2 + (o->y.p.whole - p->y.p.whole);
    if (uy > o->box3 + dx)
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
