// FUNC 8012a26c 84 X004
// MATCHING 8012a26c 84
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT TOWN_HALL_WORKER_DEFS_ANIMS[];
extern void AnimJump(TObj *, short);

void townHallWorkerSetAnim(TObj *o, short n, short k)
{
    o->anim = TOWN_HALL_WORKER_DEFS_ANIMS[o->subtype].anims[n];
    AnimJump(o, k);
}
