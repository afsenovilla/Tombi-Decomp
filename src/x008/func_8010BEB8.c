// FUNC 8010beb8 184 X008
// MATCHING 8010beb8 184
#include "gv6.h"
#include "TOBJ.H"
#include "gamestate.h"
extern unsigned char D_8009D2B0;
extern void func_8010BD08();

short func_8010BEB8(TObj *o)
{
    short r = 0;

    if (*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C6) {
        D_8009D2B0 = 0;
        func_8010BD08();
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
