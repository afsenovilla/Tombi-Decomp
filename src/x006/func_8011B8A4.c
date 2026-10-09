// FUNC 8011b8a4 408 X006
// MATCHING 8011b8a4 408
#include "TOBJ.H"

extern void *D_80122E2C[];
extern int D_1F8002D4[];
extern unsigned short D_1F800176;
extern unsigned short D_1F800186[];
extern unsigned short D_1F8000EE;
extern int D_800A4570[];
extern int D_800A4574[];
void ObjListPush_1F80022C(TObj *o);
void FUN_80018934(TObj *o);

void func_8011B8A4(TObj *o)
{
    short x;
    int x0, t4, t0;
    unsigned short k, j;
    switch (o->b04) {
    case 0:
        o->w1e = 14;
        o->y.p.whole += 0x60;
        o->b0d = 0;
        o->anim = D_80122E2C[o->subtype];
        o->b0f = 3;
        o->d3c = D_1F8002D4[0];
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        o->b04++;
        break;
    case 1:
        x0 = o->d30;
        k = D_1F800176;
        t4 = D_800A4574[0];
        j = D_1F800186[0];
        t0 = D_800A4570[0];
        o->b.p.whole = 0;
        o->a.p.whole = (short)(x0 - k + ((t4 >> 8) << 2)) >> 3;
        o->y.p.whole = (short)(o->d34 - j - ((t0 >> 8) << 3)) >> 2;
        x = o->a.p.whole;
        if (x - 0x40 < 0x141 && x + 0x40 >= 0) {
            if (o->subtype != 5 || (unsigned short)(D_1F8000EE - 0x7d0) < 0x385) {
                o->visible = 1;
                ObjListPush_1F80022C(o);
            }
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_80018934(o);
        break;
    }
}
