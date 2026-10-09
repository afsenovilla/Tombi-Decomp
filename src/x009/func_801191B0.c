// FUNC 801191b0 412 X009
// MATCHING 801191b0 412
#include "TOBJ.H"

extern unsigned char D_8009C964, D_8009C93A;
extern void func_80118BF8(TObj *);
extern void func_80118A80(TObj *);
extern void func_801174C4(TObj *);
extern void func_80118F7C(TObj *);
extern void func_8011934C(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_800187e4(TObj *);

void func_801191B0(TObj *o)
{
    switch (o->b04) {
    case 0:
        switch (o->step) {
        case 0:
            o->step++;
            break;
        case 1:
            o->b04++;
            o->step = 0;
            o->d84 = 0;
            o->d88 = 0;
            o->d8c = 0;
            *(int *)&o->w5c = o->a.p.whole;
            o->d60 = o->y.p.whole;
            o->d64 = o->b.p.whole;
            if (o->animFrame & 0x80)
                func_80118BF8(o);
            else
                func_80118A80(o);
            break;
        }
        break;
    case 1:
        if (D_8009C964 != 0x20 && D_8009C93A == 1) {
            o->b04++;
            break;
        }
        func_801174C4(o);
        if (o->animFrame & 0x80) {
            func_80118F7C(o);
        } else {
            FUN_800202b4(o);
            if (o->subtype != 0)
                func_8011934C(o);
        }
        break;
    case 2:
        if (D_8009C964 == 0x20)
            o->b04--;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
