// near-twin of stepTreasureChestGuardTalk (PAL)
// VERSION SLPS_01144 x03 8012ACAC 156
#if defined(VERSION_SLPS_01144)
extern unsigned char D_8009EA05;
#define treasureChestGuardTalk func_8012A66C
#include "gcd.h"
#include "TOBJ.H"

extern void treasureChestGuardTalk(TObj *);

void func_8012ACAC_slps01144(TObj *o)
{
    if (o->b0c != 0x28) return;
    switch (o->step) {
    case 0:
        if (o->b68) o->step++;
        if (D_8009EA05 != 0xff) break;
        if (o->step != 0) break;
        o->step = 1;
        o->state = 0x46;
        break;
    case 1:
        treasureChestGuardTalk(o);
        break;
    }
}
#endif
