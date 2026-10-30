// near-twin of swingingRopePlayerCollision (PAL)
// VERSION SLPS_01144 x01 80123AE0 248
#if defined(VERSION_SLPS_01144)
#define pushObjectOutOfObjectBoxC func_8004EDE8
#define swingingRopeOverlapTest func_8012366C_slps01144
#define decreaseObjectTimer func_8004D980
extern short D_1F80019E;
#include "spad.h"
#include "TOBJ.H"

extern short pushObjectOutOfObjectBoxC(TObj *, TObj *);
extern short swingingRopeOverlapTest(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int);

void func_80123AE0_slps01144(TObj *a, TObj *b)
{
    short r;

    if (a->step == 0x22) return;
    if (b->subtype == 7) r = pushObjectOutOfObjectBoxC(a, b);
    else r = swingingRopeOverlapTest(a, b);
    if (r == 1 && !(a->active & 2) && !(b->active & 2)) {
        a->active = 2;
        if (b->h->p.whole > a->h->p.whole) a->animFrame = 1;
        else a->animFrame = 0;
        a->b04 = 2;
        a->step = 0;
        a->state = 0;
        decreaseObjectTimer(a, 1);
        D_1F80019E = 0;
    }
}
#endif
