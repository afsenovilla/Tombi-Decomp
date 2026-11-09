// FUNC 80117cec 132 X002
// MATCHING 80117cec 132
#include "TOBJ.H"
typedef struct { void **anims; int a; int b; } AT;
typedef struct { TObj o; char pc0[0xd2 - 0xc0]; unsigned short wd2; } TX;
extern AT SEVEN_FRIENDS_NPC_DEFS_ANIMS[];
extern void AnimJump(TObj *, short);

void sevenFriendsSetAnim(TObj *o, short n, short k)
{
    if (n != ((TX *)o)->wd2) {
        o->anim = SEVEN_FRIENDS_NPC_DEFS_ANIMS[o->subtype].anims[n];
        AnimJump(o, k);
        ((TX *)o)->wd2 = n;
    }
}
