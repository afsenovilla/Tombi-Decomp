// FUNC 801258e4 88 X001
// MATCHING 801258e4 88
#include "TOBJ.H"
extern short func_80044550(TObj *, TObj *);
extern int func_800428C0(TObj *, TObj *);

void func_801258E4(TObj *o, TObj *e)
{
    if (e->subtype == 0 && func_80044550(o, e) >= 0) func_800428C0(o, e);
}
