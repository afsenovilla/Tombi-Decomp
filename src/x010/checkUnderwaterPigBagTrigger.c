// FUNC 8012eb7c 36 X010
// MATCHING 8012eb7c 36
#include "gcd.h"
#include "TOBJ.H"

void checkUnderwaterPigBagTrigger(TObj *o)
{
    if (GCD.bce1f == 0) {
        o->step = 1;
        o->state = 0;
    }
}
