// JP x10 80122450 212
// Japanese version of PAL x010 801240B8 (x010/func_801240B8.c): 128 -> 120.
#include "TOBJ.H"
extern short D_800A4582;
extern short TileCollideAt(TObj *, short, short);
extern short func_8004065C(TObj *, short, short, short);

int func_801240B8(TObj *o)
{
    if (o->b69 == 1) return 1;
    if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x12)) {
        o->b69 = 0;
        return 1;
    }
    if (D_800A4582 + 0x78 < o->y.p.whole) return 1;
    if (func_8004065C(o, o->h->p.whole - 0x10, o->y.p.whole, 1)) return 0;
    func_8004065C(o, o->h->p.whole + 0x10, o->y.p.whole, 0);
    return 0;
}
