// FUNC 801240b8 212 X010
// MATCHING 801240b8 212
// VERSION SLPS_01144 x10 80122450 212
#include "camera.h"
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL x010 801240B8 (x010/probeFloorOrWalls.c): 128 -> 120.
#include "TOBJ.H"
extern short TileCollideAt(TObj *, short, short);
extern short func_8004065C(TObj *, short, short, short);

int probeFloorOrWalls(TObj *o)
{
    if (o->b69 == 1) return 1;
    if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x12)) {
        o->b69 = 0;
        return 1;
    }
    if (CAMERA_SCROLL.maxY + 0x78 < o->y.p.whole) return 1;
    if (func_8004065C(o, o->h->p.whole - 0x10, o->y.p.whole, 1)) return 0;
    func_8004065C(o, o->h->p.whole + 0x10, o->y.p.whole, 0);
    return 0;
}
#else
#include "TOBJ.H"
extern short TileCollideAt(TObj *, short, short);
extern short probeWallAt(TObj *, short, short, short);

int probeFloorOrWalls(TObj *o)
{
    if (o->b69 == 1) return 1;
    if (TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x12)) {
        o->b69 = 0;
        return 1;
    }
    if (CAMERA_SCROLL.maxY + 0x80 < o->y.p.whole) return 1;
    if (probeWallAt(o, o->h->p.whole - 0x10, o->y.p.whole, 1)) return 0;
    probeWallAt(o, o->h->p.whole + 0x10, o->y.p.whole, 0);
    return 0;
}
#endif
