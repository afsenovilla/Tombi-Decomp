// FUNC 800eeb5c 104 X013
// MATCHING 800eeb5c 104
#include "TOBJ.H"
extern TObj *DAT_8009c330;
extern void ObjSetAnimFromTable(TObj *);
extern void FUN_8001fe94(TObj *, int);

void PlayerSetAnimIfChanged(TObj *o, unsigned short anim)
{
    TObj *p = DAT_8009c330;
    p->animTimer = anim;
    if (p->animFrame != anim) {
        p->animTimer = anim;
        ObjSetAnimFromTable(o);
        FUN_8001fe94(o, 0);
        DAT_8009c330->animFrame = DAT_8009c330->animTimer;
    }
}
