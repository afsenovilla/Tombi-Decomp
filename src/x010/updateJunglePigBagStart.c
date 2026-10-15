// FUNC 8012e494 188 X010
// MATCHING 8012e494 188
#include "gvars.h"
#include "TOBJ.H"

extern void func_8012E114(TObj *);
extern void junglePigBagEventStart(TObj *);
extern void PoolFree_1F800210(TObj *);

void updateJunglePigBagStart(TObj *o)
{
    switch (o->b04) {
    case 0:
        func_8012E114(o);
        o->b04++;
        break;
    case 1:
        if (GVARS.b3a) junglePigBagEventStart(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
