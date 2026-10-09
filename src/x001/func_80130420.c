// FUNC 80130420 432 X001
// MATCHING 80130420 432
#include "TOBJ.H"
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_8001fab4(TObj *);
extern short func_8004065C(TObj *, short, short, short);
extern short TileCollideAt(TObj *, short, short);
extern char D_80077D3C[];
extern void *D_8013E74C[];
extern short D_1F80027E;

static __inline__ int ground(TObj *o)
{
    if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 16)) {
        o->d84 = -D_1F80027E * 4 & 0xff;
        return 1;
    }
    return 0;
}

void func_80130420(TObj *o)
{
    short *w = &o->wb4;
    int d;

    switch (o->state) {
    case 0:
        o->active = 1;
        o->movetab = D_80077D3C;
        o->wac = 3;
        o->anim = D_8013E74C[0];
        AnimLoadDuration(o);
        o->wb6 = 0;
        o->state++;
    case 1:
        AnimAdvance(o);
        FUN_8001fab4(o);
        d = (o->animFrame & 1) ? -16 : 16;
        if (func_8004065C(o, o->h->p.whole + d, o->y.p.whole, o->animFrame)) {
            w[1] = 0;
            o->step = 2;
            o->state = 0;
            o->wb0 = 0;
            break;
        }
        if (!ground(o)) {
            w[1] = 0;
            o->step = 2;
            o->state = 0;
            o->wb0 = 1;
        }
        o->d8c = o->d84;
        break;
    }
    if (o->y.p.whole < -0x602 && o->h->p.whole < 0x543) {
        o->step = 5;
        o->state = 0;
    }
}
