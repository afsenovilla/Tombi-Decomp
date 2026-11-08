// VERSION SCUS_94236 x04 8011C894 692
// FLAGS -O2 -G0
#if defined(VERSION_SCUS_94236)
// USA version of JP x04 8011C14C (src/x004/updateStormBackdrop.c, VERSION_SLPS_01144 branch): US globals and callee names.
#include "camera.h"
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
extern void *D_8013405C[];
extern unsigned char D_8009C62B[];
extern unsigned char D_8009BCDD[];
extern unsigned char D_8009BCA4[];
extern int D_800A38DC[];
extern int D_800A38D8[];
extern void stormBackdropCycleClut(TObj *);
extern void func_8004FB54(short);
extern void func_8001FFE8(int);
extern void func_80018D40(TObj *);
extern void func_800188D0(TObj *);

static __inline__ void place(TObj *o)
{
    int x;
    short a;
    short y;

    {
        int d = o->d30;
        x = ((unsigned int)((d - (*(unsigned short*)((char*)&SPAD.i174 + 2)) + ((D_800A38DC[0] >> 8) << 4)) << 21)) >> 23;
    }
    a = x;
    if (x >= 0x180 && x <= 0x1c0) {
        return;
    }
    if (x > 0x1c0) {
        a = -(unsigned char)(~x + 1);
    }
    {
        unsigned short s = SPAD.s186;
        int g = D_800A38D8[0];
        y = (short)(o->d34 - s - ((g >> 8) << 4)) >> 2;
    }
    *(short *)((char *)o + 0x12) = a;
    o->b.p.whole = 0;
    o->visible = 1;
    o->y.p.whole = y;
    func_80018D40(o);
}

void func_8011C894(TObj *o)
{
    unsigned char t;
    int pa;
    int dd;
    int c;
    int k;

    t = o->b04;
    switch (t) {
    case 0:
        k = o->b0c;
        pa = o->a.p.whole;
        o->w1e = 10;
        o->b0d = 0;
        o->anim = D_8013405C[k];
        dd = ((int*)&SPAD.i2d4)[0];
        o->d30 = pa;
        o->d34 = o->y.p.whole;
        o->b6b = 0;
        o->animFrame = 0;
        o->timer = 0xe8;
        o->b04++;
        o->w22 = 0x3c;
        o->d3c = dd;
        break;
    case 1:
        if (!(D_8009C62B[0] & 8) && o->subtype) {
            stormBackdropCycleClut(o);
            switch (o->step) {
            case 0:
                if (D_8009BCDD[0] == 0) {
                    D_8009BCA4[0] = 1;
                    o->animTimer = 0x17c;
                    o->animFrame = 0x80;
                    o->step++;
                }
                break;
            case 1:
                func_8004FB54(o->animFrame);
                o->animFrame = (o->animFrame - 0x10) & 0xff;
                if (o->animFrame == 0) {
                    func_8001FFE8(0x82);
                    o->animTimer = 0x17c;
                    o->step++;
                }
                if (D_8009BCDD[0] == 3) {
                    o->step = 3;
                }
                break;
            case 2:
                c = o->animTimer;
                o->animTimer = c - 1;
                if (c == 0) {
                    o->animFrame = 0x80;
                    o->step--;
                }
                if (D_8009BCDD[0] == 3) {
                    o->step = 3;
                }
                break;
            }
        }
        place(o);
        break;
    case 2:
        o->b04 = t + 1;
        break;
    case 3:
        func_800188D0(o);
        break;
    }
}
#endif
