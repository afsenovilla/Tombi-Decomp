// FUNC 80126bbc 264 X001
// MATCHING 80126bbc 264
#include "spad.h"
#include "gcd.h"
#include "TOBJ.H"
#include "raw7.h"

extern short func_80043260(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int);

void rollingBarrelContact(TObj *o, TObj *p)
{
    p->b6a = 0;
    if (func_80043260(o, p) == 0) return;
    if (GCD.bceb0 != 0) return;
    if (p->w98 != 0) {
        if (SPAD.b1a4 != 0) return;
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
        SPAD.s19e = 0;
    } else if (U8(o, 0xc3) != 0) {
        p->b6a = o->ba6;
    }
}
