// VERSION PCPX_96084 tomba 8006F164 288
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL main0 8004874C (src/pushObjectOutOfObjectX.c): the side byte is at +0x69, not +0x9D.
#include "TOBJ.H"

int func_8006F164_pcpx96084(TObj *o, TObj *e)
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
    o->b69 = r < 0 ? 2 : 3;
    return 2;
}
#endif
