// FUNC 801295cc 132 X004
// MATCHING 801295cc 132
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
typedef struct { TObj o; char pc0[0xd2 - 0xc0]; unsigned short wd2; } TX;
extern AT D_80131188[];
extern void AnimJump(TObj *, short);

void func_801295CC(TObj *o, short n, short k)
{
    if (n != ((TX *)o)->wd2) {
        o->anim = D_80131188[o->subtype].anims[n];
        AnimJump(o, k);
        ((TX *)o)->wd2 = n;
    }
}
