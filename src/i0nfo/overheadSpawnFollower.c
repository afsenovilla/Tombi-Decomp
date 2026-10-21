// FUNC 800ee07c 116 I0NFO
// MATCHING 800ee07c 116
#include "TOBJ.H"
extern TObj *allocObjectLayer2(void);

void overheadSpawnFollower(TObj *o)
{
    TObj *n = allocObjectLayer2();
    if (n != 0) {
        n->active = 2;
        n->type = 0x19;
        n->a.raw = o->a.raw;
        n->y.raw = o->y.raw;
        n->b.raw = o->b.raw;
        n->subtype = 3;
        n->b04 = 0;
        n->step = 0;
        n->state = 0;
    }
}
