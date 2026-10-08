// FUNC 8012491c 180 X014
// MATCHING 8012491c 180
#include "TOBJ.H"

extern short D_800A457C, D_800A457E;
extern short func_8004065C(TObj *, short, short, short);

int func_8012491C(TObj *o)
{
    short d;
    short f;

    d = 8;
    if (o->velH < 0) {
        d = -8;
        f = 1;
        if (o->h->p.whole < D_800A457C - 0x98) {
            o->h->p.whole = D_800A457C - 0x98;
            return 1;
        }
    } else {
        f = 0;
        if (D_800A457E + 0x98 < o->h->p.whole) {
            o->h->p.whole = D_800A457E + 0x98;
            return 1;
        }
    }
    return func_8004065C(o, o->h->p.whole + d, o->y.p.whole, f) != 0;
}
