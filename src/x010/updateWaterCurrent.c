// FUNC 8012e9e0 388 X010
// MATCHING 8012e9e0 388
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

extern TObj PLAYER;
void PoolFree_1F800210(TObj *o);

void updateWaterCurrent(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (GAME.selectedSection == 1) {
            o->y.raw = -0xea;
        } else if (GAME.purifiedAreas & 0x40) {
            o->y.raw = -0x50;
        } else {
            o->y.raw = -0x3b8;
        }
        GVARS.s44 = 0;
        GVARS.s46 = -0x40;
        o->b04++;
        break;
    case 1: {
        TObj *p = &PLAYER;
        switch (PLAYER.step) {
        case 0x3e:
            if (PLAYER.y.p.whole < o->y.raw) {
                PLAYER.y.p.whole = o->y.raw;
                { int y = o->y.raw;
                PLAYER.step = 0x3d;
                PLAYER.state = 0;
                PLAYER.w78 = y; }
            }
            break;
        case 2:
        case 0xd:
            if (o->y.raw < p->y.p.whole) {
                p->y.p.whole = o->y.raw;
                { int y = o->y.raw;
                p->step = 0x3d;
                p->state = 0;
                p->w78 = y; }
            }
            break;
        }
        break;
    }
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
