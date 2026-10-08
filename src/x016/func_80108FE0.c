// FUNC 80108fe0 88 X016
// MATCHING 80108fe0 88
#include "TOBJ.H"
extern unsigned char D_801152E8[];
extern TObj *D_8009C330;

void func_80108FE0(TObj *o)
{
    *(signed char *)&o->b0f = -8;
    o->ba5 = 0;
    ((unsigned char *)&o->da0)[3] = 0;
    o->b9c = 0;
    *(unsigned char *)&o->wac = 0;
    o->wb2 = 0;
    o->velX = 0;
    o->velY = 0;
    o->d8c = D_801152E8[o->wb0];
    D_8009C330->timer = 0;
    o->b04 = 1;
    o->step = 0;
    o->state = 0;
}
