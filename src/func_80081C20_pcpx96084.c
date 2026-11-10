// VERSION PCPX_96084 tomba 80081C20 216
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL x001 bendingBranchGrabProbe (src/x001/bendingBranchGrabProbe.c): fields +0x9A/+0xB0/+0xBC
// (PAL +0x9E/+0xB8/+0xE8), scratchpad 0x1F8002A6, the grabbed object in the RAM global D_800E2450.
#include "spad.h"
#include "TOBJ.H"
typedef struct {
    TObj t;
} P;
extern TObj *D_800E2450;

void bendingBranchGrabProbe(P *o, TObj *e)
{
    if (e->subtype == 6
        && (unsigned short)(o->t.d->p.whole - e->d38 + 45) < 91
        && (unsigned short)(e->box0 + ((*(unsigned short *)&o->t.wbc) - e->d30)) <= e->box1
        && (unsigned short)(e->box2 + ((*(unsigned short *)&o->t.bbe) - e->d34)) <= e->box3) {
        *((unsigned char *)&o->t.w9a) = 3;
        o->t.wb0 = e->d30 - e->h->p.whole;
        o->t.wb2 = e->d34 - e->y.p.whole;
        o->t.velY = 0;
        e->b69 = 1;
        ((short *)&SPAD)[0x2A6 / 2] = 0;
        D_800E2450 = e;
    }
}
#endif
