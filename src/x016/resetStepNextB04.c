// FUNC 801174a0 24 X016
// MATCHING 801174a0 24
#include "TOBJ.H"

void resetStepNextB04(TObj *o)
{
    o->step = 0;
    o->state = 0;
    o->b04++;
}
