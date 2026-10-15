// FUNC 80119710 120 X002
// MATCHING 80119710 120
#include "gcd.h"
#include "TOBJ.H"
#include "gamestate.h"

extern unsigned int D_8009C98C;
extern void ObjSpawnType0(short, short, void *);

void spawnGoldenFlowerIfAffordable(void)
{
    Fix16 v[3];

    if (GAME.playerAP >= D_8009C98C && GCD.bcdcb != 0 && GAME.goldenFlowerState == 0) {
        v[0].p.whole = 0xb0;
        v[1].p.whole = -0x38;
        v[2].p.whole = 0;
        ObjSpawnType0(0x5f, 0, v);
    }
}
