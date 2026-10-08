// FUNC 8012a26c 84 X004
// MATCHING 8012a26c 84
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT D_801311D0[];
extern void AnimJump(TObj *, short);

void func_8012A26C(TObj *o, short n, short k)
{
    o->anim = D_801311D0[o->subtype].anims[n];
    AnimJump(o, k);
}
