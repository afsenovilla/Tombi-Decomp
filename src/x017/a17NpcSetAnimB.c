// FUNC 801180bc 84 X017
// MATCHING 801180bc 84
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT RAFT_RETURN_NPC_DEFS_ANIMS[];
extern void FUN_8001fe94(TObj *, int);

void a17NpcSetAnimB(TObj *o, unsigned short n, short t)
{
    o->anim = RAFT_RETURN_NPC_DEFS_ANIMS[o->subtype].anims[n];
    FUN_8001fe94(o, t);
}
