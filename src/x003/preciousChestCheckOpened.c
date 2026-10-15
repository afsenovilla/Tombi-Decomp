// FUNC 8012b5dc 96 X003
// MATCHING 8012b5dc 96
#include "gcd.h"
#include "TOBJ.H"


void preciousChestCheckOpened(TObj *o)
{
    if (o->b68 != 0) o->step++;
    if (GCD.bce5d == 0xff && o->step == 0) {
        o->d88 = 1;
        o->step = 1;
        o->state = 0x46;
    }
}
