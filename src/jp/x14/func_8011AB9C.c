// JP x14 8011AB9C 220
// Japanese version of PAL x014 8011D7B0 (x014/func_8011D7B0.c): 128 -> 120.
#include "TOBJ.H"
extern short D_800A4582;
extern short FUN_80040278(TObj *, short, short);
extern short FUN_8004065c(TObj *, short, short, int);

int func_8011D7B0(TObj *o)
{
    if (o->b69 == 1) {
        o->b69 = 0;
        return 1;
    }
    if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x12)) {
        o->b69 = 0;
        return 1;
    }
    if (D_800A4582 + 0x78 < o->y.p.whole) return 1;
    if (FUN_8004065c(o, o->h->p.whole - 0x10, o->y.p.whole, 1)) return 0;
    FUN_8004065c(o, o->h->p.whole + 0x10, o->y.p.whole, 0);
    return 0;
}
