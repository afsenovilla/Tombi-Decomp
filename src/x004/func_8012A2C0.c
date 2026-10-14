// FUNC 8012a2c0 92 X004
// MATCHING 8012a2c0 92
#include "gcd.h"
#include "TOBJ.H"
extern short D_1F800246;

void func_8012A2C0(TObj *o)
{
    if (GCD.bcf2d == 0 && o->b68) {
        if (D_1F800246 < 2) {
            o->state = 0;
            o->step++;
        } else {
            o->b68 = 0;
        }
    }
}
