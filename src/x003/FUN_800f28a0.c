// FUNC 800f28a0 316 X003
// MATCHING 800f28a0 316
#include "TOBJ.H"
extern unsigned short DAT_8009c960;
extern short DAT_8009c944, DAT_8009c946;
extern TObj *DAT_8009c330;
extern char DAT_80010ae8[];
extern void FUN_800efc04(TObj *);
extern void FUN_8001fe94(TObj *, int);
extern void FUN_8001fec0(TObj *);
extern void FUN_8011133c(TObj *);
extern void FUN_801113bc(TObj *);
extern void FUN_8001fd94(TObj *);
extern void FUN_800ee6d0(TObj *);

void FUN_800f28a0(TObj *o)
{
    TObj *p;
    unsigned short *q;
    if (DAT_8009c960 == 3 && DAT_8009c944 != 0) {
        p = DAT_8009c330;
        q = &p->animFrame;
        p->animTimer = 0x35;
        if (*q != 0x35) {
            p->animTimer = 0x35;
            FUN_800efc04(o);
            FUN_8001fe94(o, 0);
            DAT_8009c330->animFrame = DAT_8009c330->animTimer;
        }
    } else {
        o->anim = DAT_80010ae8;
        FUN_8001fe94(o, 3);
    }
    FUN_8001fec0(o);
    *(volatile int *)&o->h->raw += DAT_8009c944 * 0x80;
    o->y.raw += DAT_8009c946 * 0x80;
    FUN_8011133c(o);
    o->h->raw += o->velX * 0x100;
    o->timer--;
    if ((short)o->timer <= 0) {
        o->timer = 0;
        FUN_801113bc(o);
        FUN_8001fd94(o);
    }
    FUN_800ee6d0(o);
}
