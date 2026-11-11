// VERSION BETA_980203 x00 801256CC 392
#if defined(VERSION_BETA_980203)
// Beta version of src/x009/stepApBoxEvent.c (stepApBoxEvent, PAL body): AP cap 99999 (PAL 149999), timer 300 (PAL 360).
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
#include "gamestate.h"

extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);

void stepApBoxEvent(TObj *o)
{
    switch (o->step) {
    case 0:
        if (o->b69 == 1 && (*(unsigned char*)((char*)&PLAYER.obj.da0 + 2)) == 1) o->step++;
        break;
    case 1:
        if (GAME.playerAP <= 0x1869f) {
            if (GCD.bcdb4 == 0) {
                setEventStarted(0x10, 0, 0);
                o->step++;
            } else {
                o->step--;
            }
        } else if (GCD.bcdb4 != 0xff) {
            setEventComplete(0x10, 0);
            o->step++;
        } else {
            o->b04 = 2;
            o->step = 0;
        }
        break;
    case 2:
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        GVARS.b3e = 1;
        o->timer = 0x12c;
        o->step++;
        break;
    case 3:
        if (--o->timer == -1) o->step++;
        break;
    case 4:
        GVARS.b3f = 0;
        GVARS.b42 = 0;
        GVARS.b3e = 0;
        o->step = 1;
        break;
    }
}
#endif
