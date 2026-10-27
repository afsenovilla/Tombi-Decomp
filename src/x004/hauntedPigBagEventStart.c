// FUNC 8012f508 344 X004
// MATCHING 8012f508 344
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"

extern unsigned char PLAYER, PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP;
extern void setEventStarted(int, int, int);

void hauntedPigBagEventStart(TObj *o)
{
    switch (o->state) {
    case 0:
        if (GCD.bce52) {
            o->b04 = 3;
            break;
        }
        o->w08 = 0x64;
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
        GVARS.b3e = 1;
        setEventStarted(0xae, 0, 0);
        o->w08 = 200;
        o->state++;
        break;
    case 3:
        if (--o->w08 == 0) o->state = 0xf;
        break;
    case 15:
        PLAYER = 1;
        PLAYER_STATE = 1;
        GVARS.b3f = 0;
        GVARS.b42 = 0;
        GVARS.b3e = 0;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        o->b04 = 3;
        break;
    }
}
