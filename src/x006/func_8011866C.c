// FUNC 8011866c 420 X006
// MATCHING 8011866c 420
#include "TOBJ.H"

extern unsigned int FUN_8001f9e0(void);
extern int FUN_8006347c(int);

void func_8011866C(TObj *o)
{
    short y;
    short t;
    short *q;

    y = o->y.p.whole;
    q = &o->wb4;
    switch (o->w7a & 3) {
    case 0:
        o->w74 = 0;
        break;
    case 1:
        o->w74 = FUN_8001f9e0() & 3;
        if (o->wb6 == 0)
            o->w74 = 2;
        break;
    case 2:
        o->w74 = (FUN_8001f9e0() & 3) + 2;
        if (o->wb6 == 0)
            o->w74 = 4;
        break;
    case 3:
        o->w74 = 4;
        break;
    }
    t = o->w74 + o->d34;
    if (t <= y) {
        o->b69 = 1;
        o->y.p.whole += t - y;
    } else {
        o->b69 = 0;
    }
    if (o->b69 == 0) {
        o->velV += 0x28;
        if (o->velV > 0x600)
            o->velV = 0x600;
        o->y.raw += o->velV << 8;
    } else {
        o->y.raw += (FUN_8006347c(q[3]) * q[1]) >> 4;
        o->velV = 0;
        o->y.p.whole++;
    }
}
