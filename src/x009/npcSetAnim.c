// FUNC 80128af8 84 X009
// MATCHING 80128af8 84
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT FLOWER_TEARS_SEEKER_DEFS_ANIMS[];
extern void AnimJump(TObj *, short);

void npcSetAnim(TObj *o, short n, short k)
{
    o->anim = FLOWER_TEARS_SEEKER_DEFS_ANIMS[o->subtype].anims[n];
    AnimJump(o, k);
}
