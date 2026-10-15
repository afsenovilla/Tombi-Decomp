// FUNC 80125044 48 X001
// MATCHING 80125044 48
#include "TOBJ.H"

extern int pushObjectOutOfObjectBoxC(TObj *a, TObj *b);

void springVinePlayerCollision(TObj *a, TObj *b)
{
    if (b->subtype == 0)
        pushObjectOutOfObjectBoxC(a, b);
}
