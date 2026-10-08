// FUNC 8011774c 132 X018
// MATCHING 8011774c 132
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
typedef struct { TObj o; char pc0[0xd2 - 0xc0]; unsigned short wd2; } TX;
extern AT D_8011A564[];
extern void AnimJump(TObj *, short);

void func_8011774C(TObj *o, short n, short k)
{
    if (n != ((TX *)o)->wd2) {
        o->anim = D_8011A564[o->subtype].anims[n];
        AnimJump(o, k);
        ((TX *)o)->wd2 = n;
    }
}
