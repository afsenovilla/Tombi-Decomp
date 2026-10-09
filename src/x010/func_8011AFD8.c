// FUNC 8011afd8 544 X010
// MATCHING 8011afd8 544
#include "TOBJ.H"

extern unsigned char D_8009D2C3;
extern unsigned short D_8009C962;
extern void *D_80131CAC;
extern void *D_80131C64[];
extern int D_1F8002D4[];
extern unsigned short D_1F800176[];
extern unsigned short D_1F800186[];
extern int D_800A4570[];
extern int D_800A4574[];
extern void FUN_8001fe6c(TObj *);
extern void AnimAdvance(TObj *);
extern void ObjListPush_1F80022C(TObj *);
extern void FUN_80018934(TObj *);

void func_8011AFD8(TObj *o)
{
    unsigned char t;
    int x, y;
    int a, b;

    t = o->b04;
    switch (t) {
    case 0:
        o->y.p.whole += 0x20;
        o->b0d = 0;
        if (o->subtype == 99) {
            if (D_8009D2C3 & 0x40) {
                o->b04 = 3;
                break;
            }
            o->b0f = 0;
            o->anim = D_80131CAC;
            o->w1e = 12;
            FUN_8001fe6c(o);
        } else {
            o->w1e = 13;
            o->anim = D_80131C64[o->subtype];
            o->b0f = 3;
        }
        o->d3c = D_1F8002D4[0];
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        o->b04++;
        break;
    case 1:
        if (o->subtype == 99) AnimAdvance(o);
        a = o->d30;
        x = a - D_1F800176[0] + ((D_800A4574[0] >> 8) << 2);
        b = o->d34;
        y = b - D_1F800186[0] - ((D_800A4570[0] >> 8) << 3);
        switch (D_8009C962) {
        case 0: case 2: case 4: case 6:
            x = (short)x >> 2;
            y = (short)y >> 1;
            break;
        case 1: case 5:
            x = (short)x >> 1;
            y = (short)y >> 1;
            break;
        }
        o->a.p.whole = x;
        o->y.p.whole = y;
        o->b.p.whole = 0;
        if (o->a.p.whole - 0x40 < 0x141 && o->a.p.whole + 0x40 >= 0) {
            o->visible = 1;
            ObjListPush_1F80022C(o);
        }
        break;
    case 2:
        o->b04 = t + 1;
        break;
    case 3:
        FUN_80018934(o);
        break;
    }
}
