// FUNC 80118074 136 X016
// MATCHING 80118074 136
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT D_80118D34[];
extern void FUN_8001fe94(TObj *, int);

void func_80118074(TObj *o, int d, short t)
{
    short n = o->animFrame + d;
    if (n != *(unsigned short *)((char *)o + 0xd2)) {
        o->anim = D_80118D34[o->subtype].anims[n];
        FUN_8001fe94(o, t);
        *(unsigned short *)((char *)o + 0xd2) = n;
    }
}
