// FUNC 80112898 88 X000
// MATCHING 80112898 88
#include "TOBJ.H"
extern char MOVE_TABLE_0[];

void treasureChestInitCarried(TObj *o)
{
    TObj *p = *(TObj **)&o->d90;
    o->b0a = 2;
    o->movetab = MOVE_TABLE_0;
    o->d38 = 0xd20;
    o->d84 = 0;
    o->d88 = 0;
    o->d8c = 0;
    o->velX = 0;
    o->velY = 0x200;
    o->wb4 = p->h->p.whole;
    o->wb6 = p->y.p.whole;
}
