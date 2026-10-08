// FUNC 801196e0 112 X001
// MATCHING 801196e0 112
#include "TOBJ.H"
extern int D_1F8002D4[];
extern void *D_8013E754[];
extern void AnimLoadDuration(TObj *);

void func_801196E0(TObj *o)
{
    o->active = 1;
    o->type = 0x3d;
    o->b0a = 2;
    *(signed char *)&o->b0f = -0x1e;
    o->w1e = 0xb;
    o->b0b = 0;
    o->b0d = 0;
    o->d3c = D_1F8002D4[0];
    o->anim = D_8013E754[o->wac];
    AnimLoadDuration(o);
}
