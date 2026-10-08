// FUNC 80116f4c 48 X018
// MATCHING 80116f4c 48
#include "TOBJ.H"
extern void FUN_80116bdc(TObj *o);
void FUN_80116f4c(TObj *o)
{
    if (o->step == 0) FUN_80116bdc(o);
}
