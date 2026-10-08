// FUNC 801289c8 84 X003
// MATCHING 801289c8 84
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT D_80135D84[];
extern void AnimJump(TObj *, short);

void func_801289C8(TObj *o, short n, short k)
{
    o->anim = D_80135D84[o->subtype].anims[n];
    AnimJump(o, k);
}
