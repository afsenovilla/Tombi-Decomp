// FUNC 80126fec 552 X000
// MATCHING 80126fec 552
#include "TOBJ.H"
extern unsigned char DAT_800a6047[];
extern short DAT_800a60ea[];
extern void *PTR_DAT_8013b1cc, *PTR_DAT_8013b1d0;
extern int DAT_1f8002d4[];
extern void FUN_8001fe6c(TObj *o);
extern void FUN_8001fec0(TObj *o);
extern int FUN_800202b4(TObj *o);
extern void FUN_8001e4f0(int id);
extern void FUN_80118d8c(TObj *o, int x, int y, int z);
extern void FUN_80018790(TObj *o);

void FUN_80126fec(TObj *o)
{
    unsigned short t;
    unsigned char c;
    switch (o->b04) {
    case 0:
        o->b04++;
        o->box0 = 8;
        o->box1 = 0x10;
        o->box2 = 0xc;
        o->step = 0;
        o->box3 = 0x1c;
        c = DAT_800a6047[0];
        o->timer = 0;
        o->b6a = 0;
        o->b69 = 0;
        o->b0d = 0;
        o->b0f = c - 1;
        o->w1e = 9;
        o->anim = PTR_DAT_8013b1cc;
        o->d3c = DAT_1f8002d4[0];
        FUN_8001fe6c(o);
        break;
    case 1:
        if (FUN_800202b4(o) == 0)
            break;
        switch (o->step) {
        case 0:
            if (o->b69 == 1) {
                o->step = 1;
                o->b69 = 0;
                o->anim = PTR_DAT_8013b1d0;
                FUN_8001fe6c(o);
            }
            break;
        case 1:
            if (o->b69 == 0) {
                o->step = 0;
                o->anim = PTR_DAT_8013b1cc;
                o->timer = 0;
                FUN_8001fe6c(o);
            } else if (o->b69 == 1) {
                o->b69 = 0;
                t = o->timer + 1;
                o->timer = t;
                if (DAT_800a60ea[0] != 0) {
                    if (t % 32 == 0)
                        FUN_8001e4f0(0x2e);
                    else if (t % 16 == 0)
                        FUN_8001e4f0(0x2f);
                }
            }
            break;
        }
        FUN_8001fec0(o);
        break;
    case 2:
        o->b04++;
        if (o->b6a != 0)
            FUN_8001e4f0(0x31);
        FUN_80118d8c(o, (short)(o->a.p.whole + 8), (short)(o->y.p.whole - 6), o->b.p.whole);
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
