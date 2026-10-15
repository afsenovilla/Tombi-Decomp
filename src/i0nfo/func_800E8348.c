// FUNC 800e8348 80 I0NFO
// MATCHING 800e8348 80
#include "TOBJ.H"
extern void AnimJump(TObj *o, int n);
extern void **D_800EE8A4[][3];

void func_800E8348(TObj *o)
{
    o->anim = D_800EE8A4[o->subtype][0][o->w74];
    AnimJump(o, o->w76);
}
