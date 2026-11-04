// VERSION SLPS_01144 x03 80118B28 224
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"
extern void *D_80135CAC[];
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern void AnimAdvance(TObj *);
extern void ObjFree(TObj *);
void func_80118B28_slps01144(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 13;
        o->b0d = 0x80;
        o->d3c = ((int*)&SPAD.i2d4)[0];
        o->anim = D_80135CAC[o->b0c];
        AnimLoadDuration(o);
        o->d64 = 0x2000;
        break;
    case 1:
        if (ObjCullRegister(o)) AnimAdvance(o);
        break;
    case 2:
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
#endif
