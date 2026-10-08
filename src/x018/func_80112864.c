// FUNC 80112864 24 X018
// MATCHING 80112864 24
#include "TOBJ.H"

void func_80112864(TObj *o)
{
    o->b0a = 0;
    *(short *)((char *)o + 0x82) = 0;
    *(int *)((char *)o + 0x84) = 0;
    *(int *)((char *)o + 0x88) = 0;
    *(int *)((char *)o + 0x8c) = 0;
}
