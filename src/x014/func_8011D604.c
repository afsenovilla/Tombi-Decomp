// FUNC 8011d604 184 X014
// MATCHING 8011d604 184
#include "TOBJ.H"

extern short D_800A457C, D_800A457E;
extern short func_8004065C(TObj *, short, short, short);

int func_8011D604(TObj *o)
{
    short d;

    if (o->w7a & 1) {
        d = -0x10;
        if (o->h->p.whole < D_800A457C - 0x90) {
            o->h->p.whole = D_800A457C - 0x90;
            return 1;
        }
    } else {
        d = 0x10;
        if (D_800A457E + 0x90 < o->h->p.whole) {
            o->h->p.whole = D_800A457E + 0x90;
            return 1;
        }
    }
    return func_8004065C(o, o->h->p.whole + d, o->y.p.whole, o->w7a) != 0;
}
