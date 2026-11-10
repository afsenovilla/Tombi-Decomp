// VERSION PCPX_96084 tomba 8006D864 276
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Contact reaction (PAL onOverlapSetReaction3 shape): on box overlap, unless bit 1 of +0 is set, the object turns
// to face the other one and restarts in state 2.
#include "TOBJ.H"

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

void func_8006D864_pcpx96084(TObj *o, TObj *e)
{
    if (hit(o, e)) {
        if (!(o->active & 2)) {
            o->active = 2;
            if (e->h->p.whole > o->h->p.whole)
                o->animFrame = 1;
            else
                o->animFrame = 0;
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
        }
    }
}

#endif
