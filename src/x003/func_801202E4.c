// FUNC 801202e4 72 X003
// MATCHING 801202e4 72
#include "TOBJ.H"
extern short func_80044550(TObj *, TObj *);
extern void func_800428C0(TObj *, TObj *);

void func_801202E4(TObj *a, TObj *b)
{
    if (func_80044550(a, b) >= 0) func_800428C0(a, b);
}
