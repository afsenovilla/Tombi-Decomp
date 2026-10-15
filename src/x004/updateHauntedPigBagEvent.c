// FUNC 8012f69c 192 X004
// MATCHING 8012f69c 192
#include "gv2.h"
#include "TOBJ.H"
extern void hauntedPigBagEventStart(TObj *);
extern void PoolFree_1F800210(TObj *);

void updateHauntedPigBagEvent(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (GV2.bc3 & 8) {
            o->b04 = 3;
            break;
        }
        o->b04++;
        o->step = 0;
        o->state = 0;
        break;
    case 1:
        if (o->step == 0) hauntedPigBagEventStart(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
