// FUNC 801210b4 48 X010
// MATCHING 801210b4 48
#include "TOBJ.H"
extern int func_800482EC(TObj *, TObj *);
extern short D_1F80019E;

void func_801210B4(TObj *o, TObj *p)
{
    if (func_800482EC(o, p)) D_1F80019E = 0;
}
