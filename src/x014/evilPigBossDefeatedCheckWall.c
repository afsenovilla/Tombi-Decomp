// FUNC 8011d6bc 244 X014
// MATCHING 8011d6bc 244
#include "camera.h"
#include "TOBJ.H"

extern short probeWallAt(TObj *, short, short, short);
extern short TileCollideAt(TObj *, short, short);

int evilPigBossDefeatedCheckWall(TObj *o)
{
    short d;

    if (o->animFrame & 1) {
        d = -0x10;
        if (o->h->p.whole < CAMERA_SCROLL.minX - 0x90) {
            o->h->p.whole = CAMERA_SCROLL.minX - 0x90;
            return 1;
        }
    } else {
        d = 0x10;
        if (CAMERA_SCROLL.maxX + 0x90 < o->h->p.whole) {
            o->h->p.whole = CAMERA_SCROLL.maxX + 0x90;
            return 1;
        }
    }
    if (probeWallAt(o, o->h->p.whole + d, o->y.p.whole, o->animFrame & 1) == 0) {
        return TileCollideAt(o, o->h->p.whole, o->y.p.whole + 0x12) != 0;
    }
    return 1;
}
