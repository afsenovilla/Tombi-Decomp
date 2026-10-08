// FUNC 8012e1c0 772 X000
#include "TOBJ.H"
#include "raw7.h"
extern void FUN_800202b4(TObj *);
extern int FUN_800203dc(TObj *);
extern void FUN_8001fe6c(TObj *);
extern void FUN_80018790(TObj *);
extern unsigned char *FUN_800183b8(void);
extern unsigned char *FUN_800182ac(void);
extern void *PTR_DAT_8013b104;
extern int DAT_1f8002d4;
extern unsigned char DAT_8009d2ae;
extern int DAT_800a6048, DAT_800a604c, DAT_800a6050;

void FUN_8012e1c0(TObj *o)
{
    unsigned char *n;

    switch (o->b04) {
    case 0:
        switch (o->step) {
        case 0:
            o->box0 = 0x14;
            o->box1 = 0x28;
            o->box2 = 0x18;
            o->box3 = 0x30;
            o->w1e = 10;
            o->b0d = 0;
            o->b0a = 0;
            o->b69 = 0;
            o->b68 = 0;
            o->b0f = 0;
            o->d3c = DAT_1f8002d4;
            o->step++;
            if ((DAT_8009d2ae >> o->subtype) & 1) {
                n = FUN_800183b8();
                if (n != 0) {
                    n[0] = 1;
                    n[2] = 3;
                    n[3] = o->subtype;
                    n[0xc] = o->b0c;
                    S32(n, 0x10) = o->a.raw;
                    S32(n, 0x14) = o->y.raw;
                    S32(n, 0x18) = o->b.raw;
                    n[0x6b] = o->b6b;
                    S16(n, 0x7a) = 0;
                    n[0x1d] = o->b1d;
                }
                o->b04 = 3;
            }
            break;
        case 1:
            if (FUN_800203dc(o)) {
                o->step = 0;
                o->state = 0;
                o->substep = 0;
                o->b04++;
            }
            break;
        }
        break;
    case 1:
        FUN_800202b4(o);
        if (o->visible && o->step == 0 && o->state == 0) {
            o->b69 = 0;
            o->anim = PTR_DAT_8013b104;
            FUN_8001fe6c(o);
            o->state++;
        }
        break;
    case 2:
        FUN_800202b4(o);
        if (o->visible) {
            switch (o->step) {
            case 0:
                o->anim = PTR_DAT_8013b104;
                FUN_8001fe6c(o);
                o->step = 1;
                break;
            case 1:
                break;
            case 2:
                n = FUN_800182ac();
                if (n != 0) {
                    n[0] = 1;
                    n[2] = 2;
                    U16(n, 0x2e) = (o->animFrame & 1) | 2;
                    S32(n, 0x10) = DAT_800a6048;
                    S32(n, 0x14) = DAT_800a604c;
                    S32(n, 0x18) = DAT_800a6050;
                    n[3] = o->subtype;
                    n[0xc] = o->b0c;
                    n[0x6b] = o->b6b;
                    n[0x1d] = o->b1d;
                }
                o->b04 = 3;
                break;
            }
        }
        break;
    case 3:
        FUN_80018790(o);
        break;
    }
}
