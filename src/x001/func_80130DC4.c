// FUNC 80130dc4 392 X001
// MATCHING 80130dc4 392
#include "TOBJ.H"

extern char D_80077D3C[];
extern short D_1F80027E;
extern void AnimAdvance(TObj *);
extern void FUN_8001fa88(TObj *, unsigned short);
extern short func_8004065C(TObj *, short, short, short);
extern short FUN_800408d8(TObj *, short, short);

void func_80130DC4(TObj *o)
{
    short d;

    switch (o->state) {
    case 0:
        o->movetab = D_80077D3C;
        o->wb6 = 1;
        o->state++;
    case 1:
        AnimAdvance(o);
        if (o->y.p.whole < -0x5d5 && o->h->p.whole < 0x725) {
            o->step = 4;
            o->state = 0;
            break;
        }
        FUN_8001fa88(o, 1 - o->animFrame);
        o->y.p.whole -= 3;
        d = 0x10;
        if (o->animFrame & 1) d = -0x10;
        if (func_8004065C(o, o->h->p.whole + d, o->y.p.whole, o->animFrame)) {
            o->step = 2;
            o->state = 0;
            o->wb0 = 1;
        } else if (FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10)) {
            o->d8c = (-D_1F80027E * 4 + 0x80) & 0xff;
        } else {
            o->step = 2;
            o->state = 0;
            o->wb0 = 0;
        }
        break;
    }
}
