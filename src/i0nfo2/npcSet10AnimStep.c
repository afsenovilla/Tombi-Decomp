// FUNC 800fde30 48 I0NFO2
// MATCHING 800fde30 48
#include "TOBJ.H"
extern void FUN_8001fec0(TObj *o);
void npcSet10AnimStep(TObj *o)
{
    if (o->step == 0) FUN_8001fec0(o);
}
