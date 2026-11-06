// JP-only (x09), written from the disassembly
// VERSION SLPS_01144 x09 8012397C 540
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"

extern unsigned char D_8009ECC5[];
extern void *D_8012ADC8;
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void func_801237A0_slps01144(TObj *);
extern void freeObjectLayer2(TObj *);

void func_8012397C_slps01144(TObj *o)
{
    int t = o->b04;
    int s;

    switch (t) {
    case 0:
        o->active = 1;
        o->box0 = 0x10;
        o->box1 = 0x20;
        o->box2 = 8;
        o->box3 = 0x10;
        o->w1e = 9;
        o->b0d = 0;
        o->b0a = 0;
        if (D_8009ECC5[0] == 0) {
            o->b0d = 0x80;
            o->active = 2;
            o->b6b = 0x4f;
            o->b0a = 0xd;
        }
        o->d3c = SPAD.i2d4;
        o->anim = D_8012ADC8;
        *(signed char *)&o->b0f = -9;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        AnimLoadDuration(o);
        o->b04++;
        if (((unsigned char*)&GCD.bcfe5)[o->b0c]) o->b04 = 3;
        if (GCD.bcdca == 0xff) o->b04 = 3;
        break;
    case 1:
        if (ObjCullRegister(o) && o->state == 0) {
            AnimAdvance(o);
            if (GAME.selectedPlane == 0x21 && GVARS.playerLanded == 1) o->visible = 0;
        }
        break;
    case 2:
        ObjCullRegister(o);
        s = o->step;
        switch (s) {
        case 0:
        case 1:
            if (o->state == 0) AnimAdvance(o);
            break;
        case 2:
            func_801237A0_slps01144(o);
            if (o->visible == 0) o->b04 = 3;
            break;
        }
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
#endif
