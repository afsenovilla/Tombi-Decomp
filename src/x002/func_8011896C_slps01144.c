// VERSION SLPS_01144 x02 8011896C 160
// Japanese version of PAL x002 80119710 (spawnGoldenFlowerIfAffordable.c), as an object state step.
#if defined(VERSION_SLPS_01144)
#include "gcd.h"
#include "TOBJ.H"
#include "gamestate.h"

extern unsigned int D_8009C98C;
extern void ObjSpawnType0(short, short, void *);

static inline int spawnFlower(void)
{
    Fix16 v[3];
    int r = 0;

    if (GAME.playerAP >= D_8009C98C && GCD.bcdcb != 0 && GAME.goldenFlowerState == 0) {
        v[0].p.whole = 0xb0;
        v[1].p.whole = -0x38;
        v[2].p.whole = 0;
        ObjSpawnType0(0x5f, 0, v);
        r = 1;
    }
    return r;
}

void func_8011896C_slps01144(TObj *o)
{
    if (!spawnFlower()) o->b04 = 2;
}
#endif
