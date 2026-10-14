// FUNC 8012bcb8 164 X003
// MATCHING 8012bcb8 164
#include "gcd.h"
#include "TOBJ.H"

extern void treasureChestGuardTalk(TObj *);

void func_8012BCB8(TObj *o)
{
    if (o->b0c != 0x28) return;
    switch (o->step) {
    case 0:
        if (o->b68) o->step++;
        if (GCD.bce5d != 0xff) break;
        if (o->step != 0) break;
        o->d88 = 1;
        o->step = 1;
        o->state = 0x46;
        break;
    case 1:
        treasureChestGuardTalk(o);
        break;
    }
}
