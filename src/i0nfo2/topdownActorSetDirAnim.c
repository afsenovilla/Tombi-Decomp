// FUNC 800eec3c 136 I0NFO2
// MATCHING 800eec3c 136
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE94(TObj *o, int n);
extern void **TOPDOWN_ACTOR_ANIMS[][3];

void topdownActorSetDirAnim(TObj *o, int i, short n)
{
    short f = o->animFrame + i;
    if (f != U16(o, 0xD2)) {
        o->anim = TOPDOWN_ACTOR_ANIMS[o->subtype][0][f];
        func_8001FE94(o, n);
        S16(o, 0xD2) = f;
    }
}
