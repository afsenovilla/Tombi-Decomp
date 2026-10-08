// FUNC 80107fcc 412 X000
#include "TOBJ.H"
extern unsigned char *DAT_8009c330;
extern unsigned char DAT_8009cda2;
extern unsigned short DAT_1f8001c8;
extern void FUN_8001fec0(TObj *);
extern void FUN_800eeb5c(TObj *, int);

void FUN_80107fcc(TObj *o)
{
    unsigned short f;
    Fix16 *d;
    switch (o->state) {
    case 0:
        DAT_8009c330[8] = o->active;
        f = o->animFrame;
        o->velX = 0x78;
        o->active = 2;
        *((unsigned char *)o + 0xa2) = 2;
        o->d8c = 0;
        o->velY = 0;
        o->b9c = 0;
        o->b9d = 0;
        o->b9e = 0;
        o->b9f = 0;
        *((unsigned char *)o + 0xad) = 0;
        o->b69 = 0;
        *(signed char *)&o->b0f = -0x14;
        o->animFrame = f & 1;
        FUN_800eeb5c(o, 0x2b);
        o->state = o->state + 1;
    case 1:
        FUN_8001fec0(o);
        if (DAT_8009cda2 == 0)
            o->state = o->state + 1;
        break;
    case 2:
        FUN_8001fec0(o);
        if ((DAT_1f8001c8 & 1) == 0) {
            d = o->d;
            d->p.whole = d->p.whole + 5;
        } else {
            d = o->d;
            d->p.whole = d->p.whole - 5;
        }
        o->velX = o->velX - 5;
        if (o->velX != 0)
            return;
        *(signed char *)&o->b0f = -8;
        o->b9c = 0;
        o->active = DAT_8009c330[8];
        o->wb2 = 0;
        o->velX = 0;
        o->velY = 0;
        *(short *)(DAT_8009c330 + 0x20) = 0;
        break;
    }
}
