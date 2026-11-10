// VERSION PCPX_96084 tomba 8006A974 196
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Box overlap test of a against b (depth within 45, then the h and y boxes): on overlap a +0x9C = 4 and b +0x69 = 1.
#include "TOBJ.H"

void func_8006A974_pcpx96084(TObj *a, TObj *b)
{
    b->b69 = 0;
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 45) <= 90) {
        if ((unsigned short)((unsigned short)a->h->p.whole - (unsigned short)b->h->p.whole
                             + ((unsigned short)b->box0 + (unsigned short)a->box0)) <= b->box1 + a->box1) {
            if ((unsigned short)((unsigned short)a->y.p.whole - (unsigned short)b->y.p.whole
                                 + ((unsigned short)a->box2 + (unsigned short)b->box2)) <= a->box3 + b->box3) {
                a->b9c = 4;
                b->b69 = 1;
            }
        }
    }
}
#endif
