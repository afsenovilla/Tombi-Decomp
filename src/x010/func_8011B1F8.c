// FUNC 8011b1f8 312 X010
// MATCHING 8011b1f8 312
#include "spad.h"
#include "TOBJ.H"
extern void *D_80131C88;
extern unsigned short D_1F800176[];
extern int D_800A4570[];
extern void FUN_80018da4(TObj *);
extern void freeObjectLayer7(TObj *);

void func_8011B1F8(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b0d = 0;
        o->w1e = 8;
        o->y.p.whole += 0x20;
        o->anim = D_80131C88;
        o->d3c = ((int*)&SPAD.i2d4)[0];
        o->b0f = 3;
        o->d30 = o->h->p.whole;
        o->b04++;
        o->d34 = o->y.p.whole;
        break;
    case 1: {
        unsigned short cx, cy;
        int sc;
        short ax; int ay;
        cx = D_1F800176[0];
        cy = ((unsigned short*)&SPAD.s186)[0];
        sc = D_800A4570[0];
        o->b.p.whole = 0;
        o->visible = 1;
        ax = o->d30;
        ax = ax - cx;
        ax = (short)ax >> 1;
        o->a.p.whole = ax;
        ay = o->d34 - cy;
        ay -= (sc >> 8) << 3;
        o->y.p.whole = (short)ay >> 1;
        FUN_80018da4(o);
        break;
    }
    case 2:
        o->b04++;
        break;
    case 3:
        freeObjectLayer7(o);
        break;
    }
}
