// FUNC 801242c0 136 X001
// MATCHING 801242c0 136
#include "TOBJ.H"

extern short FUN_800435e0(TObj *, TObj *);

void func_801242C0(TObj *a, TObj *b)
{
    short r;

    if (b->subtype) {
        r = FUN_800435e0(a, b);
        if (r != -1 && r < 3) {
            b->active = 2;
            b->b04 = 2;
            b->b6a = 0;
            b->step = 3;
            b->state = 0;
            b->animFrame = a->animFrame & 1;
        }
    }
}
