// near-twin of PAL spawnLostSeedKidDwarf (JP: the second body after func_80135A20's return; spawns type 0x15 at (312,-284) while GCD.bcebd < 6)
// VERSION SLPS_01144 x00 80135A2C 152
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "gcd.h"
#include "TOBJ.H"
#include "gamestate.h"
extern TObj *allocObjectLayer2(void);
void func_80135A2C_slps01144(TObj *o)
{
    TObj *n;
    if (GAME.area00_eventControl & 0x40) o->b04 = 3;
    if (GCD.bcebd < 6) {
        n = allocObjectLayer2();
        if (n) {
            n->active = 1;
            n->type = 0x15;
            n->a.p.whole = 0x138;
            n->animFrame = 0;
            n->y.p.whole = -0x11c;
            n->b.p.whole = 0;
            *(TObj **)&o->category = n;
        }
        o->b04 = 1;
    } else {
        o->b04 = 3;
    }
}
#endif
