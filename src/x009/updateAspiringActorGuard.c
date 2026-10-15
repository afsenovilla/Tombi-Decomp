// FUNC 8012a70c 168 X009
// MATCHING 8012a70c 168
#include "TOBJ.H"
extern void spawnAspiringActorGuardNpcs(TObj *);
extern void aspiringActorGuardTalk(TObj *);
extern void PoolFree_1F800210(TObj *);

void updateAspiringActorGuard(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnAspiringActorGuardNpcs(o);
        o->b04++;
        break;
    case 1:
        aspiringActorGuardTalk(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
