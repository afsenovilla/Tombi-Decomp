// FUNC 800eb6ac 580 X016
// MATCHING 800eb6ac 580
#include "TOBJ.H"
extern unsigned short DAT_8009c960;
extern unsigned short DAT_80114bdc[];
extern void *DAT_80077cdc;
extern void **DAT_80115a08[];
extern void **DAT_80115948[];
extern unsigned short D_1f8001f8;
extern int D_1f800198;
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001fa88(TObj *, int);
extern void FUN_800202b4(TObj *);
extern void FUN_800187e4(TObj *);

void FUN_800eb6ac(TObj *o)
{
    unsigned short *p;
    unsigned short *g;

    switch (o->b04) {
    case 0:
        o->b04++;
        o->timer = 0x3c;
        o->movetab = &DAT_80077cdc;
        p = &DAT_80114bdc[o->subtype * 4];
        o->d8c = 0;
        o->b0d = 0;
        o->animFrame = 0;
        o->w22 = *p++;
        o->b6b = *p++;
        g = &DAT_8009c960;
        if (*(int *)g == 0x30009)
            o->anim = DAT_80115a08[o->b0c][o->b6b];
        else
            o->anim = DAT_80115948[*g * 4 + o->b0c][o->b6b];
        o->velV = *p;
        FUN_8001fe6c(o);
        break;
    case 1:
        o->velV += 0x28;
        if (o->velV > 0x400)
            o->velV = 0x400;
        o->y.raw += o->velV << 8;
        if (o->w22 & 1)
            o->d8c = (o->d8c + 8) & 0xff;
        else
            o->d8c = (o->d8c - 8) & 0xff;
        FUN_8001fa88(o, (unsigned short)o->w22);
        if (o->timer >= 0x1e || ((D_1f8001f8 + D_1f800198) & 1))
            FUN_800202b4(o);
        if (--o->timer == -1)
            o->b04 = 2;
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
