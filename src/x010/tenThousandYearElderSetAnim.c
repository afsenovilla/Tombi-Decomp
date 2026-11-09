// FUNC 80125dac 84 X010
// MATCHING 80125dac 84
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT TEN_THOUSAND_YEAR_ELDER_DEFS_ANIMS[];
extern void AnimJump(TObj *, short);

void tenThousandYearElderSetAnim(TObj *o, short n, short k)
{
    o->anim = TEN_THOUSAND_YEAR_ELDER_DEFS_ANIMS[o->subtype].anims[n];
    AnimJump(o, k);
}
