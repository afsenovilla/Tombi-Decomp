// FUNC 800eeb5c 104 X000
#include "TOBJ.H"
extern TObj *DAT_8009c330;
extern void ObjSetAnimFromTable(void);
extern void FUN_8001fe94(int, int);

void PlayerSetAnimIfChanged(int a, unsigned short anim)
{
    TObj *p = DAT_8009c330;
    p->animTimer = anim;
    if (p->animFrame != anim) {
        p->animTimer = anim;
        ObjSetAnimFromTable();
        FUN_8001fe94(a, 0);
        DAT_8009c330->animFrame = DAT_8009c330->animTimer;
    }
}
