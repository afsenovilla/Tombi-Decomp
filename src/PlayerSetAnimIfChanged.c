// FUNC 800eeb5c 104 X000
// MATCHING 800eeb5c 104
#include "TOBJ.H"
extern TObj *PLAYER_EXT;
extern void ObjSetAnimFromTable(TObj *);
extern void FUN_8001fe94(TObj *, int);

void PlayerSetAnimIfChanged(TObj *o, unsigned short anim)
{
    TObj *p = PLAYER_EXT;
    p->animTimer = anim;
    if (p->animFrame != anim) {
        p->animTimer = anim;
        ObjSetAnimFromTable(o);
        FUN_8001fe94(o, 0);
        PLAYER_EXT->animFrame = PLAYER_EXT->animTimer;
    }
}
