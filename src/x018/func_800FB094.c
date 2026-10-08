// FUNC 800fb094 44 X018
// MATCHING 800fb094 44
#include "TOBJ.H"

void func_800FB094(TObj *o)
{
    if (o->animFrame & 1)
        o->d84 = 0x100 - o->w7a;
    else
        o->d84 = o->w7a;
}
