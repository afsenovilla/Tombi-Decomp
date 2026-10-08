// FUNC 80133b9c 64 X003
// MATCHING 80133b9c 64
#include "TOBJ.H"

extern unsigned char D_8009CE4F;
extern unsigned char D_8009D0E6;

void func_80133B9C(TObj *o)
{
    if (D_8009CE4F != 0xff && D_8009D0E6 >= 5) {
        o->step = 1;
        o->state = 2;
    }
}
