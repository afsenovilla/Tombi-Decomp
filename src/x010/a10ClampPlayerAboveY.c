// FUNC 8012e918 188 X010
// MATCHING 8012e918 188
#include "TOBJ.H"
extern TObj PLAYER;
extern unsigned char PLAYER_SUBSTATE, PLAYER_STEP;
extern short PLAYER_Y, D_800A60B0;

void a10ClampPlayerAboveY(TObj *o)
{
    TObj *p = &PLAYER;

    switch (PLAYER_SUBSTATE) {
    case 0x3e:
        if (PLAYER_Y < o->y.raw) {
            PLAYER_Y = o->y.raw;
            PLAYER_SUBSTATE = 0x3d;
            PLAYER_STEP = 0;
            D_800A60B0 = o->y.raw;
        }
        break;
    case 2:
    case 0xd:
        if (o->y.raw < p->y.p.whole) {
            p->y.p.whole = o->y.raw;
            p->w78 = o->y.raw;
            p->step = 0x3d;
            p->state = 0;
        }
        break;
    }
}
