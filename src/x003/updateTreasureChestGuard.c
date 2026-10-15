// FUNC 8012bedc 304 X003
// MATCHING 8012bedc 304
#include "gcd.h"
#include "TOBJ.H"

extern void initTreasureChestGuard(TObj *);
extern void treasureChestGuardTalk(TObj *);
extern int ObjCullRegister(TObj *);
extern void freeObjectLayer2(TObj *);

void updateTreasureChestGuard(TObj *o)
{
    switch (o->b04) {
    case 0:
        initTreasureChestGuard(o);
        break;
    case 1:
        ObjCullRegister(o);
        if (o->b0c == 0x28) {
            switch (o->step) {
            case 0:
                if (o->b68 != 0) {
                    o->step++;
                }
                if (GCD.bce5d == 0xff && o->step == 0) {
                    o->step = 1;
                    o->d88 = 1;
                    o->state = 0x46;
                }
                break;
            case 1:
                treasureChestGuardTalk(o);
                break;
            }
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
