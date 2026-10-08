// FUNC 801369f8 180 X001
// MATCHING 801369f8 180
#include "TOBJ.H"
extern unsigned char D_8009CEAB;
extern int D_1F8002D4[];
extern void *D_8013E570[];
extern void AnimLoadDuration(TObj *);

void func_801369F8(TObj *o)
{
    unsigned char k = D_8009CEAB;

    if (k != 0 && k < 3) {
        o->box0 = 8;
        o->box1 = 0x10;
        o->box2 = 0x18;
        o->box3 = 0x20;
        o->d8c = 0;
        o->w1e = 10;
        o->b0d = 0;
        o->category |= 0x80;
        o->d3c = D_1F8002D4[0];
        o->anim = D_8013E570[0];
        o->b0a = 2;
        AnimLoadDuration(o);
        *(signed char *)&o->b0f = -10;
        o->b04 = 1;
    } else {
        o->b04 = 3;
    }
}
