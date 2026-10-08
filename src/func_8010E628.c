// FUNC 8010e628 68 X000
// MATCHING 8010e628 68
#include "TOBJ.H"
// volatile d88/wac stores keep the game order (matching debt)

void func_8010E628(TObj *o)
{
    int v;
    o->d84 = 0;
    v = 0x10;
    if (o->animFrame & 1) v = 0xf0;
    *(volatile int *)&o->d88 = v;
    *(volatile unsigned char *)&o->wac = 1;
    o->b9c = 2;
    o->timer = 10;
    o->step = 2;
    o->state = 3;
}
