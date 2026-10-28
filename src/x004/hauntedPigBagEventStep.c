// FUNC 8012f660 48 X004
// MATCHING 8012f660 48
#include "TOBJ.H"
extern void hauntedPigBagEventStart(TObj *o);
void FUN_8012f660(TObj *o)
{
    if (o->step == 0) hauntedPigBagEventStart(o);
}
