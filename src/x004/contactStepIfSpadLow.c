// FUNC 8012a2c0 92 X004
// MATCHING 8012a2c0 92
#include "spad.h"
#include "gcd.h"
#include "TOBJ.H"

void contactStepIfSpadLow(TObj *o)
{
    if (GCD.bcf2d == 0 && o->b68) {
        if (SPAD.s246 < 2) {
            o->state = 0;
            o->step++;
        } else {
            o->b68 = 0;
        }
    }
}
