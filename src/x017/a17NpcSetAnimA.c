// FUNC 801171d0 84 X017
// MATCHING 801171d0 84
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT D_80119990[];
extern void FUN_8001fe94(TObj *, int);

void func_801171D0(TObj *o, unsigned short n, short t)
{
    o->anim = D_80119990[o->subtype].anims[n];
    FUN_8001fe94(o, t);
}
