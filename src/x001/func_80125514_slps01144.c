// near-twin of rollingBarrelContact (PAL)
// VERSION SLPS_01144 x01 80125514 244
#if defined(VERSION_SLPS_01144)
extern unsigned char D_8009EA58;
extern short D_1F80019E;
#define pushObjectOutOfObjectBoxA func_8004E654
#define decreaseObjectTimer func_8004D980
#include "spad.h"
#include "gcd.h"
#include "TOBJ.H"
#include "raw7.h"

extern short pushObjectOutOfObjectBoxA(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int);

void func_80125514_slps01144(TObj *o, TObj *p)
{
    p->b6a = 0;
    if (pushObjectOutOfObjectBoxA(o, p) == 0) return;
    if (D_8009EA58 != 0) return;
    if (p->w98 != 0) {
        if (o->active & 2) return;
        o->active = 2;
        if (p->h->p.whole > o->h->p.whole) {
            o->animFrame = 1;
        } else {
            o->animFrame = 0;
        }
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        decreaseObjectTimer(o, 1);
        D_1F80019E = 0;
    } else if (U8(o, 0xc3) != 0) {
        p->b6a = o->ba6;
    }
}
#endif
