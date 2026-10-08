// FUNC 80112898 88 X014
// MATCHING 80112898 88
#include "TOBJ.H"
extern char D_80077CDC[];

void func_80112898(TObj *o)
{
    TObj *p = *(TObj **)&o->d90;
    o->b0a = 2;
    o->movetab = D_80077CDC;
    o->d38 = 0xd20;
    o->d84 = 0;
    o->d88 = 0;
    o->d8c = 0;
    o->velX = 0;
    o->velY = 0x200;
    o->wb4 = p->h->p.whole;
    o->wb6 = p->y.p.whole;
}
