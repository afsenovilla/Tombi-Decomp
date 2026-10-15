// FUNC 8012ed44 212 X010
// MATCHING 8012ed44 212
#include "gcd.h"
#include "TOBJ.H"
extern void underwaterPigBagEventStart(TObj *);
extern void PoolFree_1F800210(TObj *);

void updateUnderwaterPigBagStart(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->step = 0;
        o->state = 0;
        break;
    case 1:
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
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
