// FUNC 800fda2c 32 I0NFO2
// MATCHING 800fda2c 32
#include "TOBJ.H"

void resetStepAtState0_800FDA2C(TObj *o)
{
    if (o->state == 0) {
        o->step = 0;
        o->state = 0;
    }
}
