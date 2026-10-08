// FUNC 80113888 300 X004
// MATCHING 80113888 300
#include "TOBJ.H"
extern void FUN_8001fec0(TObj *o);
extern void FUN_80040278(TObj *o, int x, int y);
extern unsigned short DAT_8009c960;

void FUN_80113888(TObj *o)
{
    switch (o->state) {
    case 0:
        o->state++;
    case 1:
        FUN_8001fec0(o);
        o->h->raw += o->velX * 0x100;
        *(volatile int *)&o->y.raw += 0x40000;
        if (DAT_8009c960 == 2) {
            if (o->h->p.whole < 0x2b)
                o->h->p.whole = 0x2b;
            if (o->h->p.whole > 0x130)
                o->h->p.whole = 0x130;
        }
        FUN_80040278(o, o->h->p.whole, (short)(o->y.p.whole + o->box3 - o->box2));
        if (--o->timer == 0) {
            o->b6a = 0;
            o->step = 0;
            o->state = 0;
        }
    }
}
