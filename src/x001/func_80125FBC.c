// FUNC 80125fbc 72 X001
// MATCHING 80125fbc 72
#include "TOBJ.H"
extern short func_80044550(TObj *, TObj *);
extern void func_800428C0(TObj *, TObj *);

void func_80125FBC(TObj *a, TObj *b)
{
    if (func_80044550(a, b) >= 0) func_800428C0(a, b);
}
