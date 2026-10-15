// FUNC 80125ae4 164 X001
// MATCHING 80125ae4 164
#include "spad.h"
#include "TOBJ.H"
extern int boxesOverlapSideB(TObj *, TObj *);
extern int resolveHitResultA(TObj *, TObj *);

void springVineObjectCollision(TObj *a, TObj *b)
{
    int r;
    short s;

    if (b->subtype == 0) {
        r = boxesOverlapSideB(a, b);
        s = r;
        if (s >= 0) {
            b->w9a = (short)resolveHitResultA(a, b) >> 1;
            if (s == 2) b->animFrame = r * 2;
            else b->animFrame = (a->animFrame & 1) * 2;
            SPAD.s19e = 0;
        }
    }
}
