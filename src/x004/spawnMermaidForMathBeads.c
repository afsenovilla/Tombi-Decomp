// FUNC 8012e750 196 X004
// MATCHING 8012e750 196
#include "gcd.h"
#include "TOBJ.H"
typedef struct { unsigned short f, sub, c; short x, y, z; } Spawn;
extern Spawn D_8013143C;
extern TObj *allocObjectLayer2(void);

void spawnMermaidForMathBeads(TObj *o)
{
    TObj *e;

    if (GCD.bce22) {
        e = allocObjectLayer2();
        if (e) {
            e->active = 3;
            e->type = 0x32;
            e->animFrame = D_8013143C.f;
            e->subtype = D_8013143C.sub;
            e->b0c = D_8013143C.c;
            e->a.p.whole = D_8013143C.x;
            e->y.p.whole = D_8013143C.y;
            e->b.p.whole = D_8013143C.z;
        }
    }
    o->step = 0;
    o->state = 0;
    o->b04++;
}
