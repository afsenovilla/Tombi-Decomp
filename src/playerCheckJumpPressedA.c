// FUNC 80104ea0 140 X000
// MATCHING 80104ea0 140
// VERSION SLPS_01144 x00 80104B7C 140
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL x000 80104EA0 (playerCheckJumpPressedA.c): 64 -> 16384.
#include "gv6.h"
#include "TOBJ.H"
extern TObj *D_8009C330;
void playerCheckJumpPressedA(TObj *o)
{
    unsigned short m = *(unsigned short *)0x1f8001fc;
    if ((m & *(unsigned short *)0x1f8003c8) || (m & *(unsigned short *)0x1f8003c6)) {
        if (*(volatile unsigned short *)&GV6.s70 & 0x4000) {
            D_8009C330->animTimer = 15;
        } else {
            D_8009C330->animTimer = 14;
        }
        o->b9c = 0;
        o->velY = 0;
        o->substep = 2;
    }
}
#else
#include "gv6.h"
#include "TOBJ.H"
extern TObj *D_8009C330;
void playerCheckJumpPressedA(TObj *o)
{
    unsigned short m = *(unsigned short *)0x1f8001fc;
    if ((m & *(unsigned short *)0x1f8003c8) || (m & *(unsigned short *)0x1f8003c6)) {
        if (*(volatile unsigned short *)&GV6.s70 & 0x40) {
            D_8009C330->animTimer = 15;
        } else {
            D_8009C330->animTimer = 14;
        }
        o->b9c = 0;
        o->velY = 0;
        o->substep = 2;
    }
}
#endif
