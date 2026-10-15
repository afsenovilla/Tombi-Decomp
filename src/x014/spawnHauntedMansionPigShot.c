// FUNC 801227c0 140 X014
// MATCHING 801227c0 140
#include "spad.h"
#include "TOBJ.H"

extern short D_1F80017A[];
extern TObj *allocObjectLayer2(void);

int spawnHauntedMansionPigShot(TObj *o)
{
    TObj *n = allocObjectLayer2();

    if (n) {
        n->active = 1;
        n->type = 0x48;
        n->subtype = 0;
        n->b0c = 0;
        n->a.raw = (((short*)((char*)&SPAD.i174 + 2))[0] + 0xa0) << 16;
        n->y.raw = (D_1F80017A[0] - 0xf0) << 16;
        n->b.raw = o->b.p.whole << 16;
    }
}
