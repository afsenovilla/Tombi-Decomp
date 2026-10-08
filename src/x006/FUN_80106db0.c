// FUNC 80106db0 488 X006
// MATCHING 80106db0 488
#include "TOBJ.H"
extern void FUN_800eea3c(TObj *);
extern void FUN_800ee88c(TObj *);
extern void FUN_800ee9cc(TObj *);
extern void FUN_8001fec0(TObj *);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void SfxPlay2(int, int);
extern unsigned char DAT_801152e8[];

void FUN_80106db0(TObj *o)
{
    unsigned short t, f;
    int b, c;
    short sv;
    switch (o->state) {
    case 0:
        o->wb2 = 0;
        PlayerSetAnimIfChanged(o, 0x29);
        SfxPlay2(0x25, 0x28);
        if (*((unsigned char *)o + 0xab) & 0x80) {
            o->active = 3;
            o->timer = 0x3c;
            o->state = o->state + 1;
        } else {
            o->timer = 0x1e;
            o->state = 2;
            PlayerSetAnimIfChanged(o, 0x36);
        }
        break;
    case 1:
        FUN_800eea3c(o);
        FUN_800ee88c(o);
        FUN_800ee9cc(o);
        FUN_8001fec0(o);
        t = o->timer - 1;
        o->timer = t;
        if ((short)t <= 0) {
            o->active = 1;
            b = DAT_801152e8[o->wb0];
            o->wb2 = 0;
            *((unsigned char *)o + 0xac) = 0;
            *((unsigned char *)o + 0xab) = 0;
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->d8c = b;
        }
        break;
    case 2:
        f = o->animFrame;
        *((unsigned char *)o + 0xab) = 2;
        *((unsigned char *)o + 0xac) = 2;
        f = f & 1;
        o->animFrame = f;
        sv = 0x280;
        if (f != 0)
            sv = -0x280;
        o->wb2 = sv;
        o->state = o->state + 1;
    case 3:
        FUN_800eea3c(o);
        FUN_800ee88c(o);
        FUN_800ee9cc(o);
        FUN_8001fec0(o);
        t = o->timer - 1;
        o->timer = t;
        if ((short)t <= 0) {
            c = DAT_801152e8[o->wb0];
            o->wb2 = 0;
            *((unsigned char *)o + 0xac) = 0;
            *((unsigned char *)o + 0xab) = 0;
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->d8c = c;
        }
        break;
    }
}
