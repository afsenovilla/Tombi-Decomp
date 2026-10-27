// FUNC 800f1e48 92 I0NFO2
// MATCHING 800f1e48 92
#include "TOBJ.H"
extern void func_8001FE94(TObj *o, int n);
extern void **TOPDOWN_A11_ACTOR_ANIMS[][3];

void topdownA11ActorSetDirAnim(TObj *o, int i, short n)
{
    short f = o->animFrame + i;
    o->anim = TOPDOWN_A11_ACTOR_ANIMS[o->subtype][0][f];
    func_8001FE94(o, n);
}
