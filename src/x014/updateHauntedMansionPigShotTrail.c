// FUNC 801172dc 332 X014
// MATCHING 801172dc 332
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
extern void *D_8012A010;
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void ObjCullRegister(TObj *);
extern void FUN_800187e4(TObj *);

void updateHauntedMansionPigShotTrail(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 1;
        *(int *)((char *)o + 0x84) = 0;
        *(int *)((char *)o + 0x88) = 0;
        *(int *)((char *)o + 0x8c) = 0;
        o->b0f = 0;
        o->b0a = 0;
        o->b0d = 0x80;
        o->d3c = ((int*)&SPAD.i2dc)[0];
        o->timer = 0x28;
        o->velH = 0x100;
        if (o->animFrame) o->velH = -0x100;
        o->wac = 0;
        o->anim = D_8012A010;
        AnimLoadDuration(o);
        break;
    case 1:
        if (GVARS.b42 == 0) {
            o->a.raw += o->velH << 8;
            AnimAdvance(o);
            if (--o->timer == -1) {
                o->b04++;
            }
        }
        ObjCullRegister(o);
        break;
    case 2:
    case 3:
        FUN_800187e4(o);
        break;
    }
}
