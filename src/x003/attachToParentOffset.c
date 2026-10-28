// FUNC 80119c4c 156 X003
// MATCHING 80119c4c 156
#include "TOBJ.H"

/* frame 0x48 without saves: inline with unused locals (pad sizes found by search) */
static __inline__ void init(TObj *p)
{
    TObj *q;
    char pad[56];

    p->active = 2;
    p->d84 = 0;
    p->d88 = 0;
    p->d8c = 0;
    p->velX = 0;
    q = (TObj *)p->d90;
    if (q != 0) {
        p->d30 = p->a.raw - q->a.raw;
        p->d34 = p->y.raw - q->y.raw;
        p->d38 = p->b.raw - q->b.raw;
        if (p->d94 == 0) {
            p->box0 = 0x14;
            p->box1 = 0x28;
            p->box2 = 0xf;
            p->box3 = 0x1e;
            p->active = 1;
        }
    }
}

void func_80119C4C(TObj *o)
{
    char pad2[16];

    init(o);
}
