// FUNC 8011f128 280 X009
// MATCHING 8011f128 280
#include "gcd.h"
#include "TOBJ.H"

extern unsigned char D_8009D119;
extern unsigned char REWARD_HANDLER_BY_SUBTYPE[];
extern void (*REWARD_HANDLERS[])(TObj *);
extern void initItemObject(TObj *);
extern int ObjCullRegister(TObj *);
extern int AnimAdvance(TObj *);
extern void freeObjectLayer5(TObj *);

void updateItemPickup(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (D_8009D119 != 0 || GCD.bcf06 != 0) {
            o->b04 = 3;
            break;
        }
        initItemObject(o);
        o->active = 2;
        o->b04++;
        break;
    case 1:
        if (ObjCullRegister(o))
            AnimAdvance(o);
        break;
    case 2:
        REWARD_HANDLERS[REWARD_HANDLER_BY_SUBTYPE[o->subtype]](o);
        break;
    case 3:
        freeObjectLayer5(o);
        break;
    }
}
