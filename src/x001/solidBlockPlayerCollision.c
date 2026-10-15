// FUNC 80125074 48 X001
// MATCHING 80125074 48
#include "TOBJ.H"

extern int func_8004306C(TObj *a, TObj *b);

void solidBlockPlayerCollision(TObj *a, TObj *b)
{
    if (b->subtype == 0)
        func_8004306C(a, b);
}
