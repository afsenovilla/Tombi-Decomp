// VERSION PCPX_96084 tomba 8006C588 216
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Overlap trigger: starts state 1 of an idle object that a's box touches (box test shape of onOverlapSetReaction3).
#include "TOBJ.H"
#define B(o, n) (((unsigned char *)(o))[n])

static __inline__ short hit(TObj *o, TObj *e)
{
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 45) > 90)
        return -1;
    if ((unsigned short)(o->h->p.whole - e->h->p.whole + (e->box0 + o->box0)) > e->box1 + o->box1)
        return -1;
    if ((unsigned short)(o->y.p.whole - e->y.p.whole + (o->box2 + e->box2)) > o->box3 + e->box3)
        return -1;
    return 1;
}

void func_8006C588_pcpx96084(TObj *o, TObj *e)
{
    if (e->b04 == 0) {
        if (hit(o, e) >= 0) {
            e->b04 = 1;
            e->step = 0;
        }
    }
}
#endif
