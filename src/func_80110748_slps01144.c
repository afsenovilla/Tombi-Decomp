// VERSION SLPS_01144 x00 80110748 112
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern unsigned short D_8009F200;


void func_80110748_slps01144(TObj *o)
{
    unsigned short *pad = &D_8009F200;
    asm("" : "=r"(pad) : "0"(pad));
    if (*pad & 0x4000)
        *(unsigned short *)&o->velY += 32;
    else
        *(unsigned short *)&o->velY -= 32;
    if (o->velY > 768) o->velY = 768;
    if (o->velY < 192) o->velY = 192;
}
#endif
