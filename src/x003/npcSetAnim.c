// FUNC 801289c8 84 X003
// MATCHING 801289c8 84
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT PHOENIX_SPIRIT_HINT_NPC_DEFS_ANIMS[];
extern void AnimJump(TObj *, short);

void npcSetAnim(TObj *o, short n, short k)
{
    o->anim = PHOENIX_SPIRIT_HINT_NPC_DEFS_ANIMS[o->subtype].anims[n];
    AnimJump(o, k);
}
