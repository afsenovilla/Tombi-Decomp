// FUNC 8010e5e4 68 X008
// MATCHING 8010e5e4 68
#include "TOBJ.H"
void func_8010E5E4(TObj *o)
{
    o->velY -= 0x10;
    if (o->velY < -0x800) {
        o->velY = -0x800;
    }
    if (((unsigned char *)o)[0xcc] == 3) {
        o->state = 2;
    }
}
