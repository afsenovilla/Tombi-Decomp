// VERSION PCPX_96084 tomba 8006B7D4 204
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Box overlap contact: sets a->+0x6A and puts b in state 2 (shape of SCUS func_80052350).
#include "TOBJ.H"

void func_8006B7D4_pcpx96084(TObj *a, TObj *b)
{
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return;
    if ((unsigned short)(a->h->p.whole - b->h->p.whole + (a->box0 + b->box0)) > a->box1 + b->box1) return;
    if ((unsigned short)(a->y.p.whole - b->y.p.whole + (a->box2 + b->box2)) > a->box3 + b->box3) return;
    a->b6a = 1;
    b->active = 2;
    b->b04 = 2;
    b->step = 0;
    b->state = 0;
}
#endif
