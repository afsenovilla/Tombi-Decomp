// FUNC 8012ecd8 96 X010
// MATCHING 8012ecd8 96
#include "gcd.h"
#include "TOBJ.H"
extern void underwaterPigBagEventStart(TObj *);

void stepUnderwaterPigBagStart(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GCD.bce1f == 0) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        underwaterPigBagEventStart(o);
        break;
    }
}
