// FUNC 800ee79c 256 I0NFO
// MATCHING 800ee79c 256
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
    TObj *p;
    unsigned char s = o->b04;
    switch (s) {
    case 0: {
        int t;
        o->w1e = 0x15;
        t = SPAD.i2d8;
        p = o;
        o->b0d = 0;
        o->box0 = 4;
        o->box1 = 8;
        o->box2 = 8;
        *(signed char *)&o->b0f = -0xB;
        p->box3 = 0x20;
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
        o->b04 = s + 1;
        break;
    case 3:
        func_80018790(o);
        break;
    }
}
