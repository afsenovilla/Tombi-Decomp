// FUNC 8011d604 184 X014
// MATCHING 8011d604 184
#include "camera.h"
#include "TOBJ.H"

extern short probeWallAt(TObj *, short, short, short);

int evilPigBossCheckWall(TObj *o)
{
    short d;

    if (o->w7a & 1) {
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
    return probeWallAt(o, o->h->p.whole + d, o->y.p.whole, o->w7a) != 0;
}
