// VERSION SCUS_94236 scus94236 80055A44 348
// FLAGS -O2 -G0
#if defined(VERSION_SCUS_94236)
#include "spad.h"
#include "TOBJ.H"

extern unsigned char D_8009BCA0;
extern signed char D_8009C618;
extern short D_1F80019E;

static __inline__ short hit(TObj *a, TObj *b)
{
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return 0;
    if ((unsigned short)(a->h->p.whole - b->h->p.whole + (a->box0 + b->box0)) > a->box1 + b->box1) return 0;
    if ((unsigned short)(a->y.p.whole - b->y.p.whole + (a->box2 + b->box2)) > a->box3 + b->box3) return 0;
    return 1;
}

void func_80055A44(TObj *o)
{
    TObj **pp;
    TObj *e;

    if (D_8009BCA0 != 0) return;
    if (D_8009C618 == 3) return;
    pp = (*(TObj ***)&SPAD.p228);
    for (D_1F80019E = SPAD.s248; D_1F80019E != 0; ) {
        e = *pp++;
        D_1F80019E--;
        if (!(e->active & 3)) continue;
        if (hit(o, e)) {
            o->b6a = 1;
            e->active = 4;
            e->b04 = 2;
            e->step = 0;
            e->state = 0;
        }
    }
}
#endif
