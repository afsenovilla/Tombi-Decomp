// VERSION SCUS_94236 x06 8011ABF4 396
// FLAGS.SCUS_94236 -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SCUS_94236)
// USA version: JP C of src/x006/func_8011AA34_slps01144.c (func_8011AA34_slps01144) (1.000 masked) [-O2 -G0 -fno-strength-reduce] (tools/demo_sweep.py)
#define ObjListPush_1F80022C func_80018D40
#define freeObjectLayer7 func_800188D0
#include "spad.h"
#include "TOBJ.H"

extern struct { int rotX, rotY; } D_800A38D8;
extern void *D_801221D4[];
void ObjListPush_1F80022C(TObj *o);
void freeObjectLayer7(TObj *o);

void func_8011ABF4_scus94236(TObj *o)
{
    short x;
    int x0, t4, t0;
    unsigned short k, j;
    switch (o->b04) {
    case 0:
        o->b0d = 0;
        o->w1e = 14;
        o->anim = D_801221D4[o->subtype];
        o->b0f = 3;
        o->d3c = ((int *)&SPAD.i2d4)[0];
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        o->b04++;
        break;
    case 1:
        x0 = o->d30;
        k = (*(unsigned short *)((char *)&SPAD.i174 + 2));
        t4 = D_800A38D8.rotY;
        j = ((unsigned short *)&SPAD.s17a)[0];
        t0 = D_800A38D8.rotX;
        o->b.p.whole = 0;
        o->a.p.whole = (short)(x0 - k + ((t4 >> 8) << 2)) >> 3;
        o->y.p.whole = (short)(o->d34 - j - ((t0 >> 8) << 3)) >> 2;
        x = o->a.p.whole;
        if (x - 0x40 < 0x141 && x + 0x40 >= 0) {
            if (o->subtype != 5 || (unsigned short)((*(unsigned short *)((char *)&SPAD.iec + 2)) - 0x7d0) < 0x385) {
                o->visible = 1;
                ObjListPush_1F80022C(o);
            }
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer7(o);
        break;
    }
}
#endif
