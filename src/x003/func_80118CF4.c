// FUNC 80118cf4 260 X003
// MATCHING 80118cf4 260
#include "spad.h"
#include "gv2.h"
#include "TOBJ.H"

extern void *D_80138EA0[];
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjFree(TObj *);

void func_80118CF4(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (GV2.bc3 & 2) o->subtype += 2;
        o->w1e = 8;
        o->b0d = 0;
        o->b04++;
        o->d3c = ((int*)&SPAD.i2d4)[0];

        o->anim = D_80138EA0[o->subtype];
        AnimLoadDuration(o);
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
