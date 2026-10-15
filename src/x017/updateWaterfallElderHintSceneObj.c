// FUNC 8011974c 192 X017
// MATCHING 8011974c 192
#include "gcd.h"
#include "TOBJ.H"
extern void spawnWaterfallElderHintSceneNpcs(TObj *);
extern void waterfallElderHintScene(TObj *);
extern void FUN_80018980(TObj *);

void updateWaterfallElderHintSceneObj(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnWaterfallElderHintSceneNpcs(o);
        o->b04++;
        if (GCD.bcdbc == 0xff) o->b04 = 3;
        break;
    case 1:
        waterfallElderHintScene(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        FUN_80018980(o);
        break;
    }
}
