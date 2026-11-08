// VERSION SLPS_01144 x10 80119D9C 280
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "tobj.h"
#include "spad.h"

extern void *D_8012DE58;
void ObjListPush_1F80022C(TObj *o);
void freeObjectLayer7(TObj *o);

void func_80119D9C_slps01144(TObj *o)
{
    Fix16 *h;
    int nv;
    unsigned short k, j;
    switch (o->b04) {
    case 0:
        h = o->h;
        o->w1e = 8;
        o->b0d = 0;
        o->anim = D_8012DE58;
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
        o->a.p.whole = (short)(o->d30 - k) >> 1;
        o->y.p.whole = (short)(o->d34 - j) >> 1;
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

