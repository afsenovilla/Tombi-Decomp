// FUNC 8012d1bc 68 X010
// MATCHING 8012d1bc 68
#include "TOBJ.H"
#include "gamestate.h"
extern void spearPigAmbushSpawnStep(TObj *);

void updateSpearPigAmbushSpawner(TObj *o)
{
    if (o->subtype == 0 && GAME.selectedSection == 0) spearPigAmbushSpawnStep(o);
}
