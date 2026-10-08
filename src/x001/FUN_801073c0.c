// FUNC 801073c0 464 X001
// MATCHING 801073c0 464
#include "TOBJ.H"
typedef struct P { char pad0[8]; unsigned char b08; char pad1[0x2c - 9]; unsigned short w2c, w2e; } P;
extern P *DAT_8009c330;
extern unsigned short DAT_1f8001f8;
extern unsigned short DAT_8009c960;
extern unsigned short DAT_8009c962;
extern void FUN_800efc04();
extern void FUN_8001fe6c(TObj *);
extern void FUN_8001fec0(TObj *);
extern void FUN_800efa80(TObj *);
extern void FUN_800ef4e4(TObj *);
extern void FUN_800eea3c(TObj *);
extern void FUN_8001e560(int, int);

void FUN_801073c0(TObj *o)
{
    P *p;
    unsigned char b;
    int a;
    unsigned int d;
    if ((unsigned short)(o->wb2 + 0x144) >= 0x289) {
        p = DAT_8009c330;
        p->w2c = 0x11;
        if (p->w2e != 0x11) {
            o->d84 = 0;
            FUN_800efc04();
            FUN_8001fe6c(o);
            DAT_8009c330->w2e = DAT_8009c330->w2c;
        }
        b = o->b6b;
        o->ba4 = 1;
        if (b < 0xff)
            b += 2;
        o->b6b = b;
        if (o->animFrame & 1)
            d = (0x100 - (b >> 2)) & 0xff;
        else
            d = b >> 2;
        o->d8c = d;
        if ((DAT_1f8001f8 & 0x1f) == 0) {
            switch (DAT_8009c960) {
            case 0:
                a = 0x37;
                break;
            case 1:
                switch (DAT_8009c962) {
                case 0:
                case 1:
                    a = 0x3f;
                    break;
                case 3:
                    a = 0x59;
                    break;
                case 5:
                    a = 0x90;
                    break;
                default:
                    goto end;
                }
                break;
            default:
                goto end;
            }
            FUN_8001e560(a, 2);
        }
    } else {
        FUN_800efa80(o);
        FUN_800ef4e4(o);
        DAT_8009c330->b08 = 0;
        o->b9c = 0;
        o->d88 = 0;
        o->ba4 = 0;
        o->step = 1;
        o->state = 0;
    }
end:
    FUN_8001fec0(o);
    FUN_800eea3c(o);
    o->y.raw += 0x120000;
}
