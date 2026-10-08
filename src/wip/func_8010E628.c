// FUNC 8010e628 68 X000
#include "TOBJ.H"
// r11: score 8; sw d88 is scheduled last (game: right after the if, 2 in $v1). Tried orders/types/ternary/raw offsets.

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
