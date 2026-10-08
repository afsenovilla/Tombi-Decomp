// FUNC 8011d6bc 244 X014
// MATCHING 8011d6bc 244
#include "TOBJ.H"

extern short D_800A457C, D_800A457E;
extern short func_8004065C(TObj *, short, short, short);
extern short TileCollideAt(TObj *, short, short);

int func_8011D6BC(TObj *o)
{
    short d;

    if (o->animFrame & 1) {
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
    if (func_8004065C(o, o->h->p.whole + d, o->y.p.whole, o->animFrame & 1) == 0) {
        return TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x12) != 0;
    }
    return 1;
}
