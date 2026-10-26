// FUNC 8011d7b0 220 X014
// MATCHING 8011d7b0 220
// VERSION SLPS_01144 x14 8011AB9C 220
#include "camera.h"
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL x014 8011D7B0 (x014/func_8011D7B0.c): 128 -> 120.
#include "TOBJ.H"
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
    if (CAMERA_SCROLL.maxY + 0x78 < o->y.p.whole) return 1;
    if (FUN_8004065c(o, o->h->p.whole - 0x10, o->y.p.whole, 1)) return 0;
    FUN_8004065c(o, o->h->p.whole + 0x10, o->y.p.whole, 0);
    return 0;
}
#else
#include "TOBJ.H"
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
    if (CAMERA_SCROLL.maxY + 0x80 < o->y.p.whole) return 1;
    if (probeWallAt(o, o->h->p.whole - 0x10, o->y.p.whole, 1)) return 0;
    probeWallAt(o, o->h->p.whole + 0x10, o->y.p.whole, 0);
    return 0;
}
#endif
