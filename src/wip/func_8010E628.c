// FUNC 8010e628 68 X000
#include "TOBJ.H"

void func_8010E628(TObj *o)
{
    int v;
    o->d84 = 0;
    v = 0x10;
    if (o->animFrame & 1) v = 0xf0;
    o->d88 = v;
    *(unsigned char *)&o->wac = 1;
    o->timer = 10;
    o->b9c = o->step = 2;
    o->state = 3;
}
