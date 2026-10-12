// FUNC 8011b03c 308 X014
// MATCHING 8011b03c 308
#include "TOBJ.H"
extern void *D_8012A01C[];
extern int D_1F8002D4[];
extern unsigned short D_1F800176[];
extern unsigned short D_1F800186[];
extern void FUN_80018da4(TObj *);
extern void FUN_80018934(TObj *);

void func_8011B03C(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->w1e = 13;
        o->y.p.whole += 0x60;
        o->b0d = 0;
        o->anim = D_8012A01C[o->subtype];
        o->d3c = D_1F8002D4[0];
        o->b0f = 3;
        o->d30 = o->h->p.whole;
        o->d34 = o->y.p.whole;
        o->b04++;
        break;
    case 1: {
        unsigned short cx, cy; short ax, ay;
        cx = D_1F800176[0];
        cy = D_1F800186[0];
        o->b.p.whole = 0;
        o->visible = 1;
        ax = o->d30 - cx; o->a.p.whole = ax >> 2;
        ay = o->d34 - cy; o->y.p.whole = ay >> 2;
        FUN_80018da4(o);
        break;
    }
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018934(o);
        break;
    }
}
