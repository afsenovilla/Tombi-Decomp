// FUNC 80125dac 84 X010
// MATCHING 80125dac 84
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8012F3C4[];
extern void AnimJump(TObj *, short);

void func_80125DAC(TObj *o, short n, short k)
{
    o->anim = D_8012F3C4[o->subtype].anims[n];
    AnimJump(o, k);
}
