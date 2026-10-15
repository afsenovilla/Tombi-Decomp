// FUNC 801250a4 268 X001
// MATCHING 801250a4 268
#include "spad.h"
#include "TOBJ.H"

extern short pushObjectOutOfObjectBoxC(TObj *, TObj *);
extern short func_80124C20(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int);

void swingingRopePlayerCollision(TObj *a, TObj *b)
{
    short r;

    if (a->step == 0x22) return;
    if (b->subtype == 7) r = pushObjectOutOfObjectBoxC(a, b);
    else r = func_80124C20(a, b);
    if (r == 1 && !SPAD.b1a4 && !(a->active & 2) && !(b->active & 2)) {
        a->active = 2;
        if (b->h->p.whole > a->h->p.whole) a->animFrame = 1;
        else a->animFrame = 0;
        a->b04 = 2;
        a->step = 0;
        a->state = 0;
        decreaseObjectTimer(a, 1);
        SPAD.s19e = 0;
    }
}
