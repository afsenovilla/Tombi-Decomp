// FUNC 801243d4 60 X001
// MATCHING 801243d4 60
#include "TOBJ.H"
extern short boxesOverlap(TObj *, TObj *);

void func_801243D4(TObj *a, TObj *b)
{
    b->b6a = 0;
    if (boxesOverlap(a, b)) b->b6a = 1;
}
