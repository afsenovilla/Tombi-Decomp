// FUNC 800fb094 44 X016
// MATCHING 800fb094 44
#include "TOBJ.H"

void playerSetMoveAngleFromW7A(TObj *o)
{
    if (o->animFrame & 1)
        o->d84 = 0x100 - o->w7a;
    else
        o->d84 = o->w7a;
}
