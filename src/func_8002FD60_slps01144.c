// VERSION SLPS_01144 slps01144 8002FD60 120
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"
extern void *D_80011F00;
extern void AnimLoadDuration(TObj *);
void func_8002FD60_slps01144(TObj *o)
{
    o->active = 1;
    o->type = 49;
    o->animFrame = 1;
    o->b0a = 0;
    o->b0b = 0;
    o->b0f = 0;
    o->w1e = 20;
    o->b0d = 1;
    o->w08 = ((o->b0c + 508) << 6) | 0x17;
    {
        void *an = D_80011F00;
        int t = SPAD.i2d8;
        o->anim = an;
        o->d3c = t;
    }
    AnimLoadDuration(o);
}
#endif
