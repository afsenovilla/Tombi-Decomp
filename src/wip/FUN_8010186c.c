// FUNC 8010186c 204 X000
#include "TOBJ.H"
extern TObj *DAT_8009c330;
extern unsigned char DAT_801152e8[];
extern unsigned char DAT_8009d2af;
extern char DAT_8009c93a;
extern char LAB_80010748[];
extern void FUN_8001fe94(TObj *, int);

void FUN_8010186c(TObj *o)
{
    TObj *p;
    *(char *)((char *)DAT_8009c330 + 8) = 0;
    *(char *)((char *)o + 0xa4) = 0;
    *(char *)((char *)o + 0xa5) = 0;
    o->b9c = 0;
    *(char *)((char *)o + 0xac) = 0;
    p = DAT_8009c330;
    *(short *)((char *)o + 0xb2) = 0;
    o->velX = 0;
    o->velY = 0;
    o->d8c = DAT_801152e8[*(short *)((char *)o + 0xb0)];
    p->timer = 0;
    o->anim = LAB_80010748;
    FUN_8001fe94(o, 0);
    p = DAT_8009c330;
    p->animFrame = 0xffff;
    *(short *)((char *)p + 0x28) = 0xffff;
    *(short *)((char *)p + 0x2a) = 0xffff;
    if (DAT_8009d2af == 0) {
        DAT_8009d2af = 1;
        DAT_8009c93a = 1;
    }
    o->state = o->state + 1;
}
