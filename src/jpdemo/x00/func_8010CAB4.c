// JPDEMO x00 8010CAB4 200
// Demo version of PAL x000 80112520 (l2Type0EActive4.c): no active=1 on landing.
#include "TOBJ.H"
extern void func_800202b4(void);
extern short func_80040278(TObj *o, int a, int b);

void l2Type0EActive4(TObj *o)
{
    func_800202b4();
    switch (o->step) {
    case 0:
        o->velV = o->velV + 0x20;
        if (o->velV > 0x400) {
            o->velV = 0x400;
        }
        o->y.raw += o->velV << 8;
        if (func_80040278(o, o->h->p.whole, (short)(o->y.p.whole + 0x10)) != 0) {
            o->step = o->step + 1;
        }
        break;
    case 1:
        o->velV = 0x100;
        break;
    }
}
