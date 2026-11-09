// FUNC 80129b64 84 X004
// MATCHING 80129b64 84
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT PAINTING_SLOT_HINT_NPC_DEFS_ANIMS[];
extern void AnimJump(TObj *, short);

void paintingSlotHintNpcSetAnim(TObj *o, short n, short k)
{
    o->anim = PAINTING_SLOT_HINT_NPC_DEFS_ANIMS[o->subtype].anims[n];
    AnimJump(o, k);
}
