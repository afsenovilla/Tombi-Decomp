// FUNC 800ecb88 116 I0NFO3
// MATCHING 800ecb88 116
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE94(TObj *o, int n);
extern void **TUNNEL_ACTOR_ANIMS[][3];

void tunnelActorSetScriptAnim(TObj *o, int i, short n)
{
    short f = o->animFrame + i;
    o->anim = TUNNEL_ACTOR_ANIMS[o->subtype][0][f];
    func_8001FE94(o, n);
    S16(o, 0xD2) = f;
}
