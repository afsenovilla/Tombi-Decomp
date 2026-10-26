// FUNC 80119750 292 X001
// MATCHING 80119750 292
#include "spad.h"
#include "TOBJ.H"

extern short COS_TABLE[], NEG_SIN_TABLE[];
extern TObj *ObjAlloc(void);
extern int Rand(void);

int spawnFlowerTowerSproutSparkle(TObj *o)
{
    TObj *n;
    int r, s, i;

    if ((SPAD.s1f8 + SPAD.i198) & 1) {
        n = ObjAlloc();
        if (n) {
            n->active = 1;
            n->type = 0x31;
            n->subtype = 1;
            n->b0c = 0;
            r = Rand();
            n->h->raw = o->h->raw;
            n->y.raw = o->y.raw;
            n->d->raw = o->d->raw;
            s = ((r & 1) << 11) + 0x800;
            i = r & 0xf0;
            n->h->raw += (COS_TABLE[i] * s) >> 4;
            n->y.raw += (NEG_SIN_TABLE[i] * s) >> 4;
        }
        return ++o->timer > 0x59;
    }
    return 0;
}
