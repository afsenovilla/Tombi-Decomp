// FUNC 8010b3b8 140 X017
// MATCHING 8010b3b8 140
#include "TOBJ.H"
extern TObj *DAT_8009c330[];
extern unsigned char DAT_801152e8[];
extern void FUN_8001fe6c(TObj *o);
extern char LAB_80010748[];

void FUN_8010b3b8(TObj *o)
{
    TObj *p;
    *(char *)&DAT_8009c330[0]->w08 = 0;
    o->ba5 = 0;
    o->b9c = 0;
    *(char *)&o->wac = 0;
    p = DAT_8009c330[0];
    o->wb2 = 0;
    o->velX = 0;
    o->velY = 0;
    p->animFrame = 0xffff;
    p->timer = 0;
    p->animTimer = 0;
    o->anim = LAB_80010748;
    FUN_8001fe6c(o);
    o->d8c = DAT_801152e8[o->wb0];
    o->state = 4;
}
