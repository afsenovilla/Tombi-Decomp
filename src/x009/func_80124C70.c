// FUNC 80124c70 404 X009
// MATCHING 80124c70 404
#include "TOBJ.H"
extern void func_801174C4(TObj *);
extern void FUN_800202b4(TObj *);

void func_80124C70(TObj *o)
{
    switch (o->state) {
    case 0:
        if (o->ba7 == 1) {
            o->d34 += 0x1c0000;
            func_801174C4(o);
        } else {
            o->a.raw = o->d30;
            o->y.raw = o->d34 + 0x1c0000;
            o->b.raw = o->d38;
        }
        o->timer = 0x4b0;
        o->state++;
        break;
    case 1:
        if (--o->timer == -1) {
            o->active = 5;
            o->timer = 0x38;
            o->state++;
            if (o->ba7 == 1)
                func_801174C4(o);
        }
        break;
    case 2:
        FUN_800202b4(o);
        if (o->ba7 == 1) {
            o->d34 += -0x8000;
            func_801174C4(o);
        } else {
            o->y.raw += -0x8000;
        }
        if (--o->timer == -1) {
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->active = 1;
        }
        break;
    }
}
