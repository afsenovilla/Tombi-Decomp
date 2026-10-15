// FUNC 800ec9d4 436 I0NFO3
// MATCHING 800ec9d4 436
#include "TOBJ.H"
#include "spad.h"
#include "raw7.h"
extern void func_8001FE6C(TObj *o);
extern void func_800202B4(TObj *o);
extern void func_80018790(TObj *o);
extern void func_800EC820(TObj *o);
extern unsigned char D_800A60A3;
extern void *D_800F13D0[][8];

void func_800EC9D4(TObj *o)
{
    unsigned char s = o->b04;
    unsigned char t;
    switch (s) {
    case 0: {
        int v;
        o->b04 = s + 1;
        o->w1e = 0xF;
        v = SPAD.i2f8;
        S16(o, 0x6C) = 0x10;
        S16(o, 0x6E) = 0x20;
        S16(o, 0x70) = 0xD;
        S16(o, 0x72) = 0x1D;
        o->b0d = 0;
        S8(o, 0xF) = -0xA;
        S32(o, 0x3C) = v;
        o->anim = D_800F13D0[o->b0c][0];
        func_8001FE6C(o);
        break;
    }
    case 1:
        func_800202B4(o);
        switch (o->step) {
        case 0:
            func_800EC820(o);
            break;
        case 2:
            t = o->state;
            if (t == 0) {
                o->state = t + 1;
                o->anim = D_800F13D0[o->b0c][4];
                func_8001FE6C(o);
            }
            break;
        }
        if (D_800A60A3 != 0) {
            o->visible = 0;
        } else {
            o->visible = 1;
        }
        break;
    case 2:
        if (o->state == 0) {
            o->anim = D_800F13D0[o->b0c][0];
            func_8001FE6C(o);
            o->state++;
        }
        break;
    case 3:
        func_80018790(o);
        break;
    }
}
