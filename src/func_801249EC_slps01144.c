// VERSION SLPS_01144 x00 801249EC 256
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"

extern unsigned char D_800A7B78, D_800A7B7C, D_800A7B7D, PLAYER_STEP;
extern short boxesOverlapSigned(TObj *, TObj *);
extern int resolveHitResultD(TObj *, TObj *);
extern void func_800EB894(int, int, int, int);

void func_801249EC_slps01144(TObj *a, TObj *o)
{
    int r;
    int one;
    register unsigned short t asm("v1");
    int v;

    if (boxesOverlapSigned(a, o) == -1) return;
    if (o->b6a == 2) {
        D_800A7B78 = 1;
        D_800A7B7C = 1;
        D_800A7B7D = 0;
        PLAYER_STEP = 0;
    }
    o->b6a = 0;
    one = 1;
    r = resolveHitResultD(a, o);
    if (r) {
        if (r == one) {
            o->active = 3;
            t = a->animFrame;
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
            v = t & 1;
            goto fin;
        } else {
            func_800EB894(500, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            o->active = 2;
            t = a->animFrame;
            o->b04 = 2;
            o->step = 2;
            o->state = 0;
            v = t & 1;
            v = one - v;
        fin:
            o->animFrame = v;
        }
    }
    SPAD.s19e = 0;
}
#endif
