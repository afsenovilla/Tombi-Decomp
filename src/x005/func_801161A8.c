// FUNC 801161a8 268 X005
// MATCHING 801161a8 268
#include "TOBJ.H"

extern unsigned char D_8009D081;
extern int D_1F8002F0[];
extern void *D_8011A42C[];
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void FUN_80018790(TObj *);

void func_801161A8(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (D_8009D081 < 6) {
            o->b04 = 3;
            break;
        }
        o->b04++;
        o->box0 = 8;
        o->box1 = 0x10;
        o->box3 = 0x10;
        o->box2 = 8;
        o->b0d = 0;
        o->w1e = 7;
        o->d3c = D_1F8002F0[0];
        o->anim = D_8011A42C[o->b0c];
        AnimLoadDuration(o);
        break;
    case 1:
        AnimAdvance(o);
        ObjCullRegister(o);
        break;
    case 2:
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
