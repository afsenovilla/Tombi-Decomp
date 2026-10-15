// FUNC 80124410 144 X001
// MATCHING 80124410 144
// VERSION SLPS_01144 x01 80122F84 144
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("x01/nonmatchings/x01", func_80122F84_slps01144);
#else
#include "TOBJ.H"

typedef struct {
    TObj o;
    unsigned char pad[0x24];
    TObj *e4;
} P;

extern short objectBoxOverlapSideFacing(P *, TObj *);
extern void spawnImpactEffect(int, short, short, short);

void grabObjectContact(P *a, TObj *b)
{
    short r = objectBoxOverlapSideFacing(a, b);

    if (r != -1 && r < 3 && *(unsigned char *)&a->o.wac == 1) {
        b->active = 2;
        b->b04 = 2;
        b->step = 0;
        b->state = 0;
        b->b69 = 0;
        a->e4 = b;
        *(unsigned char *)&a->o.wac = 2;
        spawnImpactEffect(2, a->o.a.p.whole, a->o.y.p.whole, a->o.b.p.whole);
    }
}
#endif
