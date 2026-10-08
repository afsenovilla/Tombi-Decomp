// FUNC 801137bc 204 X002
// MATCHING 801137bc 204
#include "TOBJ.H"
extern void func_8001fec0(TObj *o);
extern void func_80040278(TObj *o, int a, int b);

void FUN_801137bc(TObj *o)
{
    switch (o->state) {
    case 0:
        o->velX = 0;
        o->velY = 0;
        o->velH = 0;
        o->velV = 0;
        o->state = o->state + 1;
    case 1:
        func_8001fec0(o);
        o->y.raw += 0x40000;
        func_80040278(o, o->h->p.whole, (short)(o->y.p.whole + o->box3 - o->box2));
        if (--o->timer == 0) {
            o->b6a = 0;
            o->step = 0;
            o->state = 0;
        }
    }
}
