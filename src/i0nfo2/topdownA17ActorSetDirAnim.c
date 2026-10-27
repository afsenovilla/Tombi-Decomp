// FUNC 800f3e2c 116 I0NFO2
// MATCHING 800f3e2c 116
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE94(TObj *o, int n);
extern void **TOPDOWN_A17_ACTOR_ANIMS[][3];

void topdownA17ActorSetDirAnim(TObj *o, int i, short n)
{
    short f = o->animFrame + i;
    o->anim = TOPDOWN_A17_ACTOR_ANIMS[o->subtype][0][f];
    func_8001FE94(o, n);
    S16(o, 0xD2) = f;
}
