// FUNC 80133b9c 64 X003
// MATCHING 80133b9c 64
#include "gcd.h"
#include "TOBJ.H"


void func_80133B9C(TObj *o)
{
    if (GCD.bce4f != 0xff && GCD.bd0e6 >= 5) {
        o->step = 1;
        o->state = 2;
    }
}
