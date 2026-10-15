// FUNC 80112864 24 X001
// MATCHING 80112864 24
#include "TOBJ.H"

void l2Type0EInitSubtype0(TObj *o)
{
    o->b0a = 0;
    *(short *)((char *)o + 0x82) = 0;
    *(int *)((char *)o + 0x84) = 0;
    *(int *)((char *)o + 0x88) = 0;
    *(int *)((char *)o + 0x8c) = 0;
}
