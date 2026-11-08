// VERSION SLPS_01144 x14 80118580 296
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "tobj.h"
#include "spad.h"

extern void *D_80124E70[];
extern void *D_1F8002D4;
void ObjListPush_1F80022C(TObj *o);
void freeObjectLayer7(TObj *o);

void func_80118580_slps01144(TObj *o)
{
    unsigned short k, j;
    Fix16 *h;
    int nv;
    switch (o->b04) {
    case 0:
        h = o->h;
        o->w1e = 13;
        o->b0d = 0;
        o->anim = D_80124E70[o->subtype];
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
