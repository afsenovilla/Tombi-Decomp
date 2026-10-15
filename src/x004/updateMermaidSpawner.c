// FUNC 8012e828 296 X004
// MATCHING 8012e828 296
#include "gcd.h"
#include "TOBJ.H"

typedef struct { unsigned short f, s, c, x, y, z; } T;
extern T D_8013143C;
extern TObj *allocObjectLayer2(void);
extern void PoolFree_1F800210(TObj *);

void updateMermaidSpawner(TObj *o)
{
    TObj *n;

    switch (o->b04) {
    case 0:
        if (GCD.bce22) {
            n = allocObjectLayer2();
            if (n) {
                n->active = 3;
                n->type = 0x32;
                n->animFrame = D_8013143C.f;
                n->subtype = D_8013143C.s;
                n->b0c = D_8013143C.c;
                n->a.p.whole = D_8013143C.x;
                n->y.p.whole = D_8013143C.y;
                n->b.p.whole = D_8013143C.z;
            }
        }
        o->step = 0;
        o->state = 0;
        o->b04++;
        break;
    case 1:
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
