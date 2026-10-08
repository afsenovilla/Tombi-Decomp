// FUNC 80131904 196 X001
// MATCHING 80131904 196
#include "TOBJ.H"

extern void *D_8013E690;
extern int D_1F8002D4[];
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern void FUN_80018790(TObj *);

void func_80131904(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->box0 = 8;
        o->b0d = 0;
        o->box1 = 0x10;
        o->box3 = 0xc;
        o->box2 = 0;
        o->w1e = 8;
        o->anim = D_8013E690;
        o->d3c = D_1F8002D4[0];
        o->b04++;
        AnimLoadDuration(o);
        break;
    case 1:
        ObjCullRegister(o);
        break;
    case 2:
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
