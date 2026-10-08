// FUNC 8012593c 88 X001
// MATCHING 8012593c 88
#include "TOBJ.H"
extern short func_80044550(TObj *, TObj *);
extern int func_800428C0(TObj *, TObj *);

void func_8012593C(TObj *o, TObj *e)
{
    if (e->subtype == 0 && func_80044550(o, e) >= 0) func_800428C0(o, e);
}
