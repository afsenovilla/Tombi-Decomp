// FUNC 8013bda0 168 X001
// MATCHING 8013bda0 168
#include "gcd.h"
#include "TOBJ.H"
extern void spawnLeafButterflySwarm(TObj *);
extern void stepLeafButterflySwarm(TObj *);
extern void PoolFree_1F800210(TObj *);

void updateLeafButterflySwarm(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnLeafButterflySwarm(o);
        break;
    case 1:
        { int x = GCD.bce56; x ^= 0xff; if (x) stepLeafButterflySwarm(o); }
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
