// VERSION SLPS_01144 x00 801106F8 80
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern unsigned short D_8009F200;


void func_801106F8_slps01144(TObj *a)
{
    TObj *o = a;
    volatile unsigned short *pad = &D_8009F200;
    if (*pad & 0x8000)
        o->velX = -0x200;
    else if (*pad & 0x2000)
        o->velX = 0x200;
    else
        o->velX = 0;
}
#endif
