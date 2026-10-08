// FUNC 8011c890 220 X004
// MATCHING 8011c890 220
#include "TOBJ.H"

extern int D_1F8002D4[];
extern void *D_80134D80;
extern void AnimLoadDuration(TObj *);
extern int FUN_80020078(TObj *, int);
extern void ObjFreeDup(TObj *);

void func_8011C890(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->b0d = 0;
        o->w1e = 8;
        o->animFrame = 1;
        o->anim = D_80134D80;
        o->d3c = D_1F8002D4[0];
        o->box0 = 4;
        o->box1 = 8;
        o->box2 = 0xa0;
        o->box3 = 0xc4;
        AnimLoadDuration(o);
        break;
    case 1:
        FUN_80020078(o, 0x60);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
