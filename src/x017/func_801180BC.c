// FUNC 801180bc 84 X017
// MATCHING 801180bc 84
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8011999C[];
extern void FUN_8001fe94(TObj *, int);

void func_801180BC(TObj *o, unsigned short n, short t)
{
    o->anim = D_8011999C[o->subtype].anims[n];
    FUN_8001fe94(o, t);
}
