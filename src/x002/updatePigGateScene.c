// FUNC 8011c628 168 X002
// MATCHING 8011c628 168
#include "TOBJ.H"
extern void spawnPigGateSceneNpcs(TObj *);
extern void stepPigGateScene(TObj *);
extern void PoolFree_1F800210(TObj *);

void updatePigGateScene(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnPigGateSceneNpcs(o);
        o->b04++;
        break;
    case 1:
        stepPigGateScene(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
