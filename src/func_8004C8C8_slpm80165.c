// VERSION SLPM_80165 slpm80165 8004C8C8 316
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of the beta func_800544AC (src/func_800544AC_beta980203.c): no D_8009C618 check, the hit enemy gets
// active = 6, the three box tests continue the loop directly.
#include "spad.h"
#include "TOBJ.H"

extern unsigned char D_8009BCA0;
extern short D_1F80019E;

void func_8004C8C8(TObj *o)
{
    TObj **pp;
    TObj *e;

    if (D_8009BCA0 != 0) return;
    pp = (*(TObj ***)&SPAD.p228);
    for (D_1F80019E = SPAD.s248; D_1F80019E != 0; ) {
        e = *pp++;
        D_1F80019E--;
        if (!(e->active & 3)) continue;
        if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) >= 0x5b) continue;
        if ((unsigned short)(o->h->p.whole - e->h->p.whole + (o->box0 + e->box0)) > o->box1 + e->box1) continue;
        if ((unsigned short)(o->y.p.whole - e->y.p.whole + (o->box2 + e->box2)) > o->box3 + e->box3) continue;
        o->b6a = 1;
        e->active = 6;
        e->b04 = 2;
        e->step = 0;
        e->state = 0;
    }
}
#endif
