// FUNC 800f3eec 80 X010
// MATCHING 800f3eec 80
#include "TOBJ.H"
extern unsigned char *D_8009C330;

void func_800F3EEC(TObj *o)
{
    D_8009C330[8] = 1;
    D_8009C330[7] = o->animFrame;
    o->b9c = 2;
    o->timer = 10;
    o->step = 4;
    o->velY = 0;
    *(unsigned char *)&o->wac = 1;
    o->state = 3;
}
