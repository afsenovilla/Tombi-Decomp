// FUNC 800ecbbc 80 I0NFO
// MATCHING 800ecbbc 80
#include "TOBJ.H"
extern void func_8001FE94(TObj *o, int n);
extern void **OVERHEAD_ACTOR_ANIMS[][3];

void overheadActorSetScriptAnim(TObj *o)
{
    o->anim = OVERHEAD_ACTOR_ANIMS[o->subtype][0][o->w74];
    func_8001FE94(o, o->w76);
}
