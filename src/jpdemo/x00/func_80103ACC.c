// JPDEMO x00 80103ACC 396
// Demo version of PAL x000 801231F4 (FUN_801231f4.c): player animation change inline (PAL calls PlayerSetAnimIfChanged); frame id 0xf8 (PAL 0xbd).
#include "tobj.h"
extern TObj *D_8009C330_anim;
extern void func_800EDA04(TObj *);
extern void func_80020554(TObj *);
#define f3(o, n) {                                                   \
    TObj *p_ = D_8009C330_anim;                                       \
    p_->animTimer = (n);                                              \
    if (p_->animFrame != (n)) {                                       \
        func_800EDA04((TObj *)(o));                                   \
        func_80020554((TObj *)(o));                                   \
        D_8009C330_anim->animFrame = D_8009C330_anim->animTimer;      \
    }                                                                 \
}
extern void f1(TObj *), f4(int, int, int), f5(int, int);
void FUN_801231f4(TObj *o)
{
    switch (o->state) {
    case 0:
        o->d8c = 0;
        o->velX = 0;
        o->velY = 0;
        o->wb6 = 0;
        f3(o, 0x22);
        o->state++;
    case 1:
        f1(o);
        if (*(unsigned short *)o->anim == 0xf8) {
            f4(o->a.p.whole, o->y.p.whole, o->b.p.whole);
            f4((short)(o->a.p.whole + 1), (short)(o->y.p.whole - 2), o->b.p.whole);
            f4((short)(o->a.p.whole - 2), (short)(o->y.p.whole + 2), o->b.p.whole);
            f4((short)(o->a.p.whole + 2), o->y.p.whole, o->b.p.whole);
            f5(0, 4);
            o->state++;
        }
        break;
    case 2:
        f1(o);
        break;
    }
}
