// FUNC 8011d7b0 220 X014
// MATCHING 8011d7b0 220
#include "TOBJ.H"
extern short D_800A4582;
extern short FUN_80040278(TObj *, short, short);
extern short probeWallAt(TObj *, short, short, int);

int evilPigBossCheckLanded(TObj *o)
{
    if (o->b69 == 1) {
        o->b69 = 0;
        return 1;
    }
    if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x12)) {
        o->b69 = 0;
        return 1;
    }
    if (D_800A4582 + 0x80 < o->y.p.whole) return 1;
    if (probeWallAt(o, o->h->p.whole - 0x10, o->y.p.whole, 1)) return 0;
    probeWallAt(o, o->h->p.whole + 0x10, o->y.p.whole, 0);
    return 0;
}
