// FUNC 8010b3b8 140 X000
#include "TOBJ.H"
extern TObj *DAT_8009c330;
extern unsigned char DAT_801152e8[];
extern void FUN_8001fe6c(void);
extern char LAB_80010748[];

void FUN_8010b3b8(TObj *o)
{
    TObj *p;
    *(char *)((char *)DAT_8009c330 + 8) = 0;
    *(char *)((char *)o + 0xa5) = 0;
    *(char *)((char *)o + 0x9c) = 0;
    *(char *)((char *)o + 0xac) = 0;
    p = DAT_8009c330;
    *(short *)((char *)o + 0xb2) = 0;
    o->velX = 0;
    o->velY = 0;
    p->animFrame = 0xffff;
    p->timer = 0;
    p->animTimer = 0;
    o->anim = LAB_80010748;
    FUN_8001fe6c();
    o->d8c = DAT_801152e8[*(short *)((char *)o + 0xb0)];
    o->state = 4;
}
