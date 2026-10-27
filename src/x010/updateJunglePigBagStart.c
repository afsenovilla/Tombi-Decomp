// FUNC 8012e494 188 X010
// MATCHING 8012e494 188
#include "gvars.h"
#include "TOBJ.H"

extern void spawnJunglePigBagStartNpcs(TObj *);
extern void junglePigBagEventStart(TObj *);
extern void PoolFree_1F800210(TObj *);

void updateJunglePigBagStart(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnJunglePigBagStartNpcs(o);
        o->b04++;
        break;
    case 1:
        if (GVARS.playerLanded) junglePigBagEventStart(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
