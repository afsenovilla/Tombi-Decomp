// FUNC 8012e8c8 80 X010
// MATCHING 8012e8c8 80
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

void func_8012E8C8(TObj *o)
{
    if (GAME.selectedSection == 1) o->y.raw = -0xea;
    else if (GAME.purifiedAreas & 0x40) o->y.raw = -0x50;
    else o->y.raw = -0x3b8;
    GVARS.driftH = 0;
    GVARS.driftY = -0x40;
}
