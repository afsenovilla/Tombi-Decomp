// FUNC 80129194 188 X004
// MATCHING 80129194 188
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
