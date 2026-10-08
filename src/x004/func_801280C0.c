// FUNC 801280c0 84 X004
// MATCHING 801280c0 84
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8013117C[];
extern void AnimJump(TObj *, short);

void func_801280C0(TObj *o, short n, short k)
{
    o->anim = D_8013117C[o->subtype].anims[n];
    AnimJump(o, k);
}
