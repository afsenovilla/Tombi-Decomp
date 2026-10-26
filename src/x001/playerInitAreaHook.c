// FUNC 801212a0 304 X001
// MATCHING 801212a0 304
#include "gcd.h"
#include "TOBJ.H"
#include "gamestate.h"
extern TObj *GRABBED_OBJECT;
extern void spawnHeldPuppyBaron(int, int, int, int);
extern void spawnCaughtForestDwarf(int, int, int);
extern void FUN_80026e0c(int, int);

void playerInitAreaHook(TObj *o)
{
    GRABBED_OBJECT = 0;
    if (GAME.baronState == 3)
        spawnHeldPuppyBaron(0, o->a.p.whole, o->y.p.whole, o->b.p.whole);
    if (GCD.bcdb2 != 0xff) {
        if (!(GAME.area00_eventControl & 0x100))
            goto clr;
        if (GAME.selectedSection == 3 || GAME.selectedSection == 1)
            spawnCaughtForestDwarf(o->a.p.whole, o->y.p.whole, o->b.p.whole);
    } else {
        if (!(GAME.area00_eventControl & 0x100))
            goto clr;
        GAME.area00_eventControl &= ~0x100;
    }
    if (!(GAME.area00_eventControl & 0x100)) {
    clr:
        GCD.bcef7 = 0;
    }
    if (GCD.bcdb8 == 0xff && GCD.bd0be != 0)
        FUN_80026e0c(0x1a, 1);
}
