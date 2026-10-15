// FUNC 800ee140 288 I0NFO3
// MATCHING 800ee140 288
#include "TOBJ.H"
#include "spad.h"
extern void func_80018790(TObj *o);
extern void func_8001FE6C(TObj *o);
extern void func_800202B4(TObj *o);
extern void func_800EDF28(TObj *o);
extern void *D_8001219C[];
extern short D_800EED84[][4];

void func_800EE140(TObj *o)
{
    switch (o->b04) {
    case 0: {
        int t;
        o->w1e = 0x15;
        t = SPAD.i2d8;
        o->box0 = 4;
        o->box1 = 8;
        o->box2 = 8;
        o->box3 = 0x20;
        o->b0d = 0;
        *(signed char *)&o->b0f = -0xB;
        o->d3c = t;
        o->anim = D_8001219C[D_800EED84[o->subtype][0]];
        func_8001FE6C(o);
        o->b68 = 0;
        o->b69 = 0;
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        break;
    }
    case 1:
        func_800202B4(o);
        func_800EDF28(o);
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        func_80018790(o);
        break;
    }
}
