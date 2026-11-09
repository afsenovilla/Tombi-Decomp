// FUNC 801171d0 84 X017
// MATCHING 801171d0 84
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT WATERFALL_HINT_NPC_DEFS_ANIMS[];
extern void FUN_8001fe94(TObj *, int);

void a17NpcSetAnimA(TObj *o, unsigned short n, short t)
{
    o->anim = WATERFALL_HINT_NPC_DEFS_ANIMS[o->subtype].anims[n];
    FUN_8001fe94(o, t);
}
