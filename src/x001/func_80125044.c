// FUNC 80125044 48 X001
// MATCHING 80125044 48
#include "TOBJ.H"

extern int func_800439F4(TObj *a, TObj *b);

void func_80125044(TObj *a, TObj *b)
{
    if (b->subtype == 0)
        func_800439F4(a, b);
}
