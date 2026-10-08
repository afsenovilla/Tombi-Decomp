// FUNC 80126c50 180 X004
// MATCHING 80126c50 180
#include "TOBJ.H"
extern void FUN_80026bfc(int, int);
extern void AnimLoadDuration(TObj *);
extern void *D_80134614[];

static __inline__ void set_box(TObj *o, short a, short b, short c, short d)
{
    o->box0 = a;
    o->box1 = b;
    o->box2 = c;
    o->box3 = d;
}

void func_80126C50(TObj *o)
{
    switch (o->state) {
    case 0:
        set_box(o, 0x14, 0x28, 0, 0x14);
        *(signed char *)&o->b0f = -7;
        o->b68 = 0;
        o->d8c = 0;
        FUN_80026bfc(2, 6);
        o->state++;
        break;
    case 2:
    case 3:
    case 4:
    case 7:
        o->wac = 0x18;
        o->anim = D_80134614[0];
        AnimLoadDuration(o);
        break;
    case 1:
        break;
    }
}
