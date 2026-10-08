// FUNC 800f369c 68 X009
// MATCHING 800f369c 68
#include "TOBJ.H"

void func_800F369C(TObj *o)
{
    int v = 0x10;
    o->timer = 10;
    o->d84 = 0;
    if (o->animFrame & 1)
        v = 0xf0;
    *(unsigned char *)&o->wac = 1;
    o->b9c = 2;
    o->d88 = v;
    o->d8c = 0;
    o->state = 3;
}
