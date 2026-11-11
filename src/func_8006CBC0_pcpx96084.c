// VERSION PCPX_96084 tomba 8006CBC0 312
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Bump: when a (not in state bit 2) overlaps b, b reacts (+0x69 = 1) and a's speed +0xAA, if faster than 256,
// is braked by 48 but kept at 256 or more.
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

void func_8006CBC0_pcpx96084(TObj *a, TObj *b)
{
    if (a->active & 2) return;
    if (hit(a, b)) {
        b->b69 = 1;
        if ((unsigned short)(a->waa + 256) > 512) {
            if (a->waa < 0) {
                a->waa += 48;
                if (a->waa >= -255) a->waa = -256;
            } else {
                a->waa -= 48;
                if (a->waa < 256) a->waa = 256;
            }
        }
    }
}
#endif
