// VERSION SLPS_01144 x14 80116058 332
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"
extern void *D_80124E64[];
extern unsigned char D_8009E4EA;
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void ObjFree(TObj *);
void func_80116058_slps01144(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 1;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b0f = 0;
        o->b0a = 0;
        o->b0d = 0x80;
        o->d3c = SPAD.i2dc;
        o->timer = 40;
        if (o->animFrame) o->velH = -0x100;
        else o->velH = 0x100;
        o->anim = D_80124E64[0];
        o->wac = 0;
        AnimLoadDuration(o);
        break;
    case 1:
        if (D_8009E4EA == 0) {
            o->a.raw += o->velH << 8;
            AnimAdvance(o);
            if (--o->timer == -1) o->b04++;
        }
        ObjCullRegister(o);
        break;
    case 2:
    case 3:
        ObjFree(o);
        break;
    }
}
#endif
