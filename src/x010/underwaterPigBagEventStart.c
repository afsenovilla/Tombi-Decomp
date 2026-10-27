// FUNC 8012eba0 312 X010
// MATCHING 8012eba0 312
#include "gvars.h"
#include "TOBJ.H"
extern unsigned char PLAYER, PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP;
extern void setEventStarted(int, int, int);

void underwaterPigBagEventStart(TObj *o)
{
    switch (o->state) {
    case 0:
        o->state++;
        break;
    case 1:
        if (GVARS.b3a == 0) break;
        PLAYER_STATE = 5;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        o->state++;
        break;
    case 2:
        setEventStarted(0x7b, 0, 0);
        o->w08 = 200;
        o->state++;
        break;
    case 3:
        if (--o->w08 == 0) o->state = 15;
        break;
    case 15:
        GVARS.b3f = 0;
        GVARS.b42 = 0;
        GVARS.b3e = 0;
        PLAYER = 1;
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
