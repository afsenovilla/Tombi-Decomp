// FUNC 8012f4f0 24 X004
// MATCHING 8012f4f0 24
#include "TOBJ.H"

void resetStepNextB04(TObj *o)
{
    o->step = 0;
    o->state = 0;
    o->b04++;
}
