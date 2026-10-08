// FUNC 801320a8 204 X000
// MATCHING 801320a8 204
#include "TOBJ.H"
extern void FUN_80026bfc(int, int);
extern void FUN_8001fe6c(TObj *);
extern void *DAT_8013ad40[];
extern void *DAT_8013ad44[];

static __inline__ void set_box(TObj *o, short a, short b, short c, short d)
{
    o->box0 = a;
    o->box1 = b;
    o->box2 = c;
    o->box3 = d;
}

void FUN_801320a8(TObj *o)
{
    switch (o->state) {
    case 0: {
        set_box(o, 0x14, 0x28, 0, 0x14);
        *(signed char *)&o->b0f = -7;
        o->b68 = 0;
        o->d8c = 0;
        FUN_80026bfc(0, 6);
        o->state++;
        break; }
    case 3:
        o->wac = 4;
        o->anim = DAT_8013ad40[0];
        FUN_8001fe6c(o);
        break;
    case 2:
    case 4:
    case 7:
        o->wac = 5;
        o->anim = DAT_8013ad44[0];
        FUN_8001fe6c(o);
        break;
    }
}
