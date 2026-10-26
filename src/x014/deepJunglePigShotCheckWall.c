// FUNC 8012491c 180 X014
// MATCHING 8012491c 180
#include "camera.h"
#include "TOBJ.H"

extern short probeWallAt(TObj *, short, short, short);

int deepJunglePigShotCheckWall(TObj *o)
{
    short d;
    short f;

    d = 8;
    if (o->velH < 0) {
        d = -8;
        f = 1;
        if (o->h->p.whole < CAMERA_SCROLL.minX - 0x98) {
            o->h->p.whole = CAMERA_SCROLL.minX - 0x98;
            return 1;
        }
    } else {
        f = 0;
        if (CAMERA_SCROLL.maxX + 0x98 < o->h->p.whole) {
            o->h->p.whole = CAMERA_SCROLL.maxX + 0x98;
            return 1;
        }
    }
    return probeWallAt(o, o->h->p.whole + d, o->y.p.whole, f) != 0;
}
