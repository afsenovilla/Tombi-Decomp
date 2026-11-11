// VERSION SCUS_94236 x14 8011A38C 296
// FLAGS.SCUS_94236 -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SCUS_94236)
// USA version: JP C of src/x014/func_80118580_slps01144.c (func_80118580_slps01144) (1.000 masked) [-O2 -G0 -fno-strength-reduce] (tools/demo_sweep.py)
#include "tobj.h"
#include "spad.h"

extern void *D_801293C4[];
extern void *D_1F8002D4;
void ObjListPush_1F80022C(TObj *o);
void freeObjectLayer7(TObj *o);

void func_8011A38C_scus94236(TObj *o)
{
    unsigned short k, j;
    Fix16 *h;
    int nv;
    switch (o->b04) {
    case 0:
        h = o->h;
        o->w1e = 13;
        o->b0d = 0;
        o->anim = D_801293C4[o->subtype];
        o->d3c = SPAD.i2d4;
        o->b0f = 3;
        nv = h->p.whole;
        o->b04++;
        o->d30 = nv;
        o->d34 = o->y.p.whole;
        break;
    case 1:
        k = *(unsigned short *)((char *)&SPAD.i174 + 2);
        j = SPAD.s17a;
        asm("" : "=r"(k), "=r"(j) : "0"(k), "1"(j));
        o->visible = 1;
        o->b.p.whole = 0;
        o->a.p.whole = (short)(o->d30 - k) >> 2;
        o->y.p.whole = (short)(o->d34 - j) >> 2;
        ObjListPush_1F80022C(o);
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
