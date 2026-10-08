// FUNC 80128af8 84 X009
// MATCHING 80128af8 84
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8012B2E4[];
extern void AnimJump(TObj *, short);

void func_80128AF8(TObj *o, short n, short k)
{
    o->anim = D_8012B2E4[o->subtype].anims[n];
    AnimJump(o, k);
}
