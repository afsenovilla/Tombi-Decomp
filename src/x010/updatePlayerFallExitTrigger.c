// FUNC 8012d858 140 X010
// MATCHING 8012d858 140
#include "TOBJ.H"
extern void stepPlayerFallExitScene(TObj *);
extern void PoolFree_1F800210(TObj *);

void updatePlayerFallExitTrigger(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        break;
    case 1:
        stepPlayerFallExitScene(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
