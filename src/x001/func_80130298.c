// FUNC 80130298 392 X001
// MATCHING 80130298 392
#include "TOBJ.H"

extern short D_1F80027E;
extern short D_1F80016A;
extern void *D_8013E740[];
extern short FUN_80040278(TObj *, short, short);
extern void AnimLoadDuration(TObj *);

static __inline__ int land(TObj *o)
{
    if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) {
        o->d84 = (-D_1F80027E << 2) & 0xff;
        return 1;
    }
    return 0;
}

void func_80130298(TObj *o)
{
    switch (o->state) {
    case 0:
        o->animFrame = 1;
        o->y.p.whole += 4;
        land(o);
        o->state++;
        o->d8c = 0;
        o->wb0 = 0;
        o->wac = 0;
        o->anim = D_8013E740[0];
        AnimLoadDuration(o);
    case 1:
        o->y.p.whole += 4;
        land(o);
        if (o->visible) o->state++;
        break;
    case 2:
        if (o->h->p.whole < D_1F80016A) {
            o->state = 0;
            o->animFrame = 0;
            o->step++;
        }
        break;
    }
}
