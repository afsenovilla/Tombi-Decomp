// FUNC 80138c4c 256 X001
// MATCHING 80138c4c 256
#include "TOBJ.H"
extern int D_1F8002D4[];
extern void *D_8013E72C[];
extern int ObjCullRegister(TObj *);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void FUN_80018790(TObj *);

void func_80138C4C(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        o->active = 2;
        o->w1e = 7;
        *(signed char *)&o->b0f = -9;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->b0d = 0x80;
        o->d3c = D_1F8002D4[0];
        o->anim = D_8013E72C[0];
        AnimLoadDuration(o);
        o->b04++;
        break;
    case 1:
        ObjCullRegister(o);
        if (AnimAdvance(o)) o->b04++;
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
