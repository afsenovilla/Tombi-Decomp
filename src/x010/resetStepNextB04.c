// FUNC 8012eb64 24 X010
// MATCHING 8012eb64 24
#include "TOBJ.H"

void resetStepNextB04(TObj *o)
{
    o->step = 0;
    o->state = 0;
    o->b04++;
}
