// FUNC 8010beb8 184 X017
// MATCHING 8010beb8 184
#include "gv2.h"
#include "gv6.h"
#include "TOBJ.H"
#include "gamestate.h"
extern void playerReleaseClingObject();

short playerTryStartJump(TObj *o)
{
    short r = 0;

    if (*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C6) {
        GV2.bb0 = 0;
        playerReleaseClingObject();
        if (GAME.area00_eventControl & 0x40) {
            if (*(volatile unsigned short *)&GV6.s70 & *(unsigned short *)0x1F8003C4) {
                o->ba7 = 1;
            }
        }
        r++;
        o->b9c = 1;
        o->step = 2;
        o->state = 0;
    }
    return r;
}
