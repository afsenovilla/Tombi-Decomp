// FUNC 80129b64 84 X004
// MATCHING 80129b64 84
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT D_801311B8[];
extern void AnimJump(TObj *, short);

void func_80129B64(TObj *o, short n, short k)
{
    o->anim = D_801311B8[o->subtype].anims[n];
    AnimJump(o, k);
}
