// FUNC 80134c20 572 X000
// MATCHING 80134c20 572
#include "TOBJ.H"
extern int DAT_1f8002d0[];
extern void *PTR_8013ac18[];
extern void *PTR_8013ac34[];
extern void *PTR_8013ac3c[];
extern unsigned char DAT_8009ce4a;
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001fec0(TObj *);
extern void FUN_800202b4(TObj *);
extern void FUN_80134980(TObj *);
extern void FUN_801346c0(TObj *);
extern void FUN_80018790(TObj *);

void FUN_80134c20(TObj *o)
{
    int st;
    void *an;

    switch (o->b04) {
    case 0:
        o->box0 = 10;
        o->box1 = 0x14;
        o->box2 = 0x10;
        o->box3 = 0x20;
        o->w1e = 1;
        o->b0a = 2;
        *(signed char *)&o->b0f = -8;
        o->b0d = 0;
        o->d3c = DAT_1f8002d0[0];
        o->anim = PTR_8013ac18[0];
        FUN_8001fe6c(o);
        o->d8c = 0;
        o->category |= 0x80;
        o->b04++;
        if (DAT_8009ce4a == 0xff)
            o->b04 = 3;
        break;
    case 1:
        if (o->subtype == 0x63) {
            FUN_800202b4(o);
            FUN_80134980(o);
            break;
        }
        if (DAT_8009ce4a == 0xff) {
            o->active = 2;
            FUN_800202b4(o);
            if (o->visible == 0)
                o->b04 = 3;
        } else {
            FUN_800202b4(o);
        }
        break;
    case 2:
        FUN_800202b4(o);
        switch (o->step) {
        case 0:
            st = o->state;
            if (st == 0) goto c0;
        chk:
            if (st == 1) goto fec;
            return;
        c0:
            an = PTR_8013ac34[0];
            goto set;
        case 1:
            st = o->state;
            if (st != 0) goto chk;
            an = PTR_8013ac3c[0];
        set:
            o->anim = an;
            FUN_8001fe6c(o);
            o->state++;
            goto fec;
        case 2:
            o->b04 = 3;
            return;
        case 3:
            FUN_801346c0(o);
            return;
        case 4:
            break;
        default:
            return;
        }
    fec:
        FUN_8001fec0(o);
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
