// FUNC 801242c0 136 X001
// MATCHING 801242c0 136
#include "TOBJ.H"

extern short objectBoxOverlapSideFacing(TObj *, TObj *);

void forestDwarfContact(TObj *a, TObj *b)
{
    short r;

    if (b->subtype) {
        r = objectBoxOverlapSideFacing(a, b);
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
