// FUNC 80130f4c 336 X001
// MATCHING 80130f4c 336
#include "TOBJ.H"

extern short D_1F80027E;
extern short FUN_80040278(TObj *, short, short);
extern int AnimAdvance(TObj *);

static __inline__ int land(TObj *o)
{
    if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) {
        o->d84 = (-D_1F80027E << 2) & 0xff;
        return 1;
    }
    return 0;
}

void func_80130F4C(TObj *o)
{
    switch (o->state) {
    case 0:
        o->wb6 = 0;
        o->velV = 0;
        o->state++;
        break;
    case 1:
        o->d8c += 8;
        if (o->d8c > 0x100) {
            o->d8c = 0;
            o->state++;
        }
    case 2:
        AnimAdvance(o);
        o->velV += 0x30;
        if (o->velV > 0x400) {
            o->velV = 0x400;
        }
        o->y.raw += o->velV << 8;
        if (land(o)) {
            o->step = 1;
            o->state = 0;
            o->animFrame = 1;
        }
        break;
    }
}
