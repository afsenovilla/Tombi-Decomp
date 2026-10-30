// near-twin of playerTryStartJump (PAL)
// VERSION SLPS_01144 x00 8010B994 176
#if defined(VERSION_SLPS_01144)
extern unsigned int D_8009E52C;
extern unsigned short D_8009F200;
#define playerReleaseClingObject func_8010B7E4_slps01144
#include "gv2.h"
#include "gv6.h"
#include "TOBJ.H"
#include "gamestate.h"
extern void playerReleaseClingObject();

short func_8010B994_slps01144(TObj *o)
{
    short r = 0;

    if (*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C6) {
        playerReleaseClingObject();
        if (D_8009E52C & 0x40) {
            if (*(volatile unsigned short *)&D_8009F200 & *(unsigned short *)0x1F8003C4) {
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
#endif
