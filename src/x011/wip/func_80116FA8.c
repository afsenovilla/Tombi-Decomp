// FUNC 80116fa8 228 X011
/* score 8: only case 0 scheduling: game loads li v1,0x10 first (box0/box2) and sh 0xac late; tried hill-climb/random store orders, local v */
#include "TOBJ.H"

extern void *D_8011C420;
extern int D_1F8002F0[];
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern void func_80116E54(TObj *);
extern void FUN_80018790(TObj *);

void func_80116FA8(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->box1 = 0x20;
        o->box3 = 0x20;
        o->b0d = 1;
        o->w08 = 0x7952;
        o->box2 = 0x10;
        o->w1e = 9;
        o->wac = 0;
        o->d3c = D_1F8002F0[0];
        o->box0 = 0x10;
        o->anim = D_8011C420;
        AnimLoadDuration(o);
        break;
    case 1:
        ObjCullRegister(o);
        func_80116E54(o);
        break;
    case 2:
    case 3:
        FUN_80018790(o);
        break;
    }
}
