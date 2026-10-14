// FUNC 801210b4 48 X010
// MATCHING 801210b4 48
#include "spad.h"
#include "TOBJ.H"
extern int func_800482EC(TObj *, TObj *);

void func_801210B4(TObj *o, TObj *p)
{
    if (func_800482EC(o, p)) SPAD.s19e = 0;
}
