// VERSION BETA_980203 x00 8010FDA8 176
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/func_8010B994_slps01144.c (func_8010B994_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
extern unsigned int D_8009E52C_beta980203;
extern unsigned short D_8009F200_beta980203;
#define playerReleaseClingObject playerReleaseClingObject
#include "gv2.h"
#include "gv6.h"
#include "TOBJ.H"
#include "gamestate.h"
extern void playerReleaseClingObject();

short func_8010FDA8_beta980203(TObj *o)
{
    short r = 0;

    if (*(unsigned short *)0x1F8001FC & *(unsigned short *)0x1F8003C6) {
        playerReleaseClingObject();
        if (D_8009E52C_beta980203 & 0x40) {
            if (*(volatile unsigned short *)&D_8009F200_beta980203 & *(unsigned short *)0x1F8003C4) {
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
