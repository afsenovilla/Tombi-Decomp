// FUNC 801300b0 272 X003
// MATCHING 801300b0 272
#include "TOBJ.H"

extern void *D_80138FD8[];
extern int D_1F800300[];
extern unsigned short GetClut(int, int);
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern void func_8012FCAC(TObj *);
extern void FUN_80018790(TObj *);

void func_801300B0(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->active = 3;
        o->box0 = 6;
        o->box1 = 0xc;
        o->box2 = 8;
        o->box3 = 0x10;
        o->w1e = 4;
        o->b0d = 1;
        o->w08 = GetClut(0xa0, 0x1e1);
        o->wac = 6;
        o->anim = D_80138FD8[0];
        o->d3c = D_1F800300[0];
        AnimLoadDuration(o);
        break;
    case 1:
        ObjCullRegister(o);
        func_8012FCAC(o);
        break;
    case 2:
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
