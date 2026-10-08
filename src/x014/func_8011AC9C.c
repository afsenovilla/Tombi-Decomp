// FUNC 8011ac9c 220 X014
// MATCHING 8011ac9c 220
#include "TOBJ.H"

extern int D_1F8002D4[];
extern void *D_80129F90;
extern unsigned short D_80125CA0[];
extern void AnimLoadDuration(TObj *);
extern int ObjCullRegister(TObj *);
extern void ObjFreeDup(TObj *);

void func_8011AC9C(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->box0 = 2;
        o->box1 = 6;
        o->box2 = 0x38;
        o->box3 = D_80125CA0[o->b0c];
        o->w1e = 0x10;
        o->b0d = 0;
        o->d3c = D_1F8002D4[0];
        o->anim = D_80129F90;
        AnimLoadDuration(o);
        break;
    case 1:
        ObjCullRegister(o);
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
