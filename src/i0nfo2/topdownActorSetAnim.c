// FUNC 800eebe8 84 I0NFO2
// MATCHING 800eebe8 84
#include "TOBJ.H"
extern void func_8001FE94(TObj *o, int n);
extern void **TOPDOWN_ACTOR_ANIMS[][3];

void topdownActorSetAnim(TObj *o, unsigned short i, short n)
{
    o->anim = TOPDOWN_ACTOR_ANIMS[o->subtype][0][i];
    func_8001FE94(o, n);
}
