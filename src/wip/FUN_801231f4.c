// FUNC 801231f4 336 X000
#include "tobj.h"
extern void f1(TObj *), f3(TObj *, int), f4(int, int, int), f5(int, int);
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
        if (*(unsigned short *)o->anim == 0xbd) {
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
