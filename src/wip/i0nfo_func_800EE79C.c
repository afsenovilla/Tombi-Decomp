// FUNC 800ee79c 256 I0NFO
#include "TOBJ.H"
#include "spad.h"
#include "raw7.h"
extern void func_8001FE6C(TObj *o, int a);
extern void func_800202B4(TObj *o);
extern void func_800EE5E4(TObj *o);
extern void func_80018790(TObj *o);
extern void *D_8001219C;
void func_800EE79C(TObj *o)
{
    switch (o->b04) {
    case 0: {
        int t;
        o->w1e = 0x15;
        t = SPAD.i2d8;
        o->box0 = 4;
        o->box1 = 8;
        o->box2 = 8;
        o->b0d = 0;
        S8(o, 0xF) = -0xB;
        o->box3 = 0x20;
        o->d3c = t;
        o->anim = D_8001219C;
        func_8001FE6C(o, t);
        o->b68 = 0;
        o->b69 = 0;
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        break;
    }
    case 1:
        func_800202B4(o);
        func_800EE5E4(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80018790(o);
        break;
    }
}
