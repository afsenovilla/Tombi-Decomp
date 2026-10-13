// FUNC 8001fd30 24 MAIN0
// MATCHING 8001fd30 24
#include "TOBJ.H"

void applyObjectSpeedVertical(TObj *o)
{
    o->y.raw += o->velV << 8;
}
