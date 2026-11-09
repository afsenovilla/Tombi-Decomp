// FUNC 80118074 136 X016
// MATCHING 80118074 136
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
extern AT JUICE_SELLER_DEFS_ANIMS[];
extern void FUN_8001fe94(TObj *, int);

void a16ActorSetAnimIfChanged(TObj *o, int d, short t)
{
    short n = o->animFrame + d;
    if (n != *(unsigned short *)((char *)o + 0xd2)) {
        o->anim = JUICE_SELLER_DEFS_ANIMS[o->subtype].anims[n];
        FUN_8001fe94(o, t);
        *(unsigned short *)((char *)o + 0xd2) = n;
    }
}
