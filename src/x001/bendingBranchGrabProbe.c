// FUNC 80126e8c 216 X001
// MATCHING 80126e8c 216
// VERSION SLPS_01144 x01 801257D0 216
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("x01/nonmatchings/x01", func_801257D0_slps01144);
#else
#include "spad.h"
#include "TOBJ.H"
typedef struct {
    TObj t;
    char pad[0xe8 - 0xc0];
    unsigned short e8;
    unsigned short ea;
} P;

void bendingBranchGrabProbe(P *o, TObj *e)
{
    if (e->subtype == 6
        && (unsigned short)(o->t.d->p.whole - e->d38 + 45) < 91
        && (unsigned short)(e->box0 + (o->e8 - e->d30)) <= e->box1
        && (unsigned short)(e->box2 + (o->ea - e->d34)) <= e->box3) {
        o->t.b9e = 3;
        o->t.wb8 = e->d30 - e->h->p.whole;
        o->t.wba = e->d34 - e->y.p.whole;
        o->t.velY = 0;
        e->b69 = 1;
        SPAD.s19e = 0;
        (*(TObj **)&SPAD.p3c0) = e;
    }
}
#endif
