// VERSION SLPM_80165 x00 800EE21C 224
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of playerInitAreaHook (src/x001/playerInitAreaHook.c) without the event-flag clear and the
// FUN_80026e0c call at the end.
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
            return;
        if (GAME.selectedSection == 3 || GAME.selectedSection == 1)
            spawnCaughtForestDwarf(o->a.p.whole, o->y.p.whole, o->b.p.whole);
    } else {
        if (GAME.area00_eventControl & 0x100)
            GAME.area00_eventControl &= ~0x100;
    }
}
#endif
