// FUNC 80129194 188 X004
// MATCHING 80129194 188
// VERSION SLPS_01144 x04 80127944 168
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern unsigned char D_8009E9CA, D_8009E4E8, D_8009E4E9, D_8009E9E5;
extern short PLAYER_Y;
void func_80127944(TObj *o)
{
    if (D_8009E9CA == 0) return;
    if (D_8009E4E8 && D_8009E4E9 == 0x94) {
        o->step = 2;
        o->state = 0;
    }
    if (PLAYER_Y >= -0xa8 && D_8009E9E5 != 0 && D_8009E9E5 != 0xff) {
        o->step = 3;
        o->state = 0;
    }
    if (o->b68) {
        o->animFrame = o->b68 & 1;
        o->step = 1;
        o->state = 0;
    }
}
#else
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
#include "gamestate.h"


void a04ItemTriggerCheck(TObj *o)
{
    if (GCD.bce22 == 0) return;
    if (GVARS.useItemPending && GVARS.useItemId == 0x94) {
        o->step = 2;
        o->state = 0;
    }
    if (PLAYER.obj.y.p.whole >= -0xa8 && GAME.displayExpBar != 1 && GCD.bce3d != 0 && GCD.bce3d != 0xff) {
        o->step = 3;
        o->state = 0;
    }
    if (o->b68) {
        o->animFrame = o->b68 & 1;
        o->step = 1;
        o->state = 0;
    }
}
#endif
