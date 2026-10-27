// FUNC 80129b70 296 X009
// MATCHING 80129b70 296
#include "plx.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern void stepPlayerWalkInScene(TObj *);
extern void PoolFree_1F800210(TObj *);

static __inline__ short Cond(void)
{
    if (!GVARS.playerLanded) return 0;
    if (GVARS.hurtState) return 0;
    if (!PLAYER.obj.b69 && !PLX.b6100) return 0;
    if ((unsigned int)((unsigned short)(*(Fix16 **)&PLAYER.obj.h)->p.whole - 0x1034) >= 0xc8) return 0;
    if (PLAYER.obj.y.p.whole < -0x1d0) return 0;
    return 1;
}

void updatePlayerWalkInTrigger(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04 = 1;
        o->step = 0;
        o->state = 0;
        break;
    case 1:
        if (Cond()) stepPlayerWalkInScene(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
