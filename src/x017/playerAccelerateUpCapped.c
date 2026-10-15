// FUNC 8010e5e4 68 X017
// MATCHING 8010e5e4 68
#include "TOBJ.H"
void playerAccelerateUpCapped(TObj *o)
{
    o->velY -= 0x10;
    if (o->velY < -0x800) {
        o->velY = -0x800;
    }
    if (((unsigned char *)o)[0xcc] == 3) {
        o->state = 2;
    }
}
