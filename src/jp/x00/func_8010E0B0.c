// JP x00 8010E0B0 68
// Japanese version of PAL x000 8010E5E4 (playerAccelerateUpCapped.c): -16 -> -80.
#include "TOBJ.H"
void playerAccelerateUpCapped(TObj *o)
{
    o->velY -= 0x50;
    if (o->velY < -0x800) {
        o->velY = -0x800;
    }
    if (((unsigned char *)o)[0xcc] == 3) {
        o->state = 2;
    }
}
