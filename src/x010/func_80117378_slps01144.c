// VERSION SLPS_01144 x10 80117378 820
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"

extern unsigned char D_8009E500[];
extern unsigned char D_8009EE6B, D_8009EAAE, D_8009E4E7, D_8009E4E0, D_8009E4E6;
extern void playSFX(int);
extern short func_80042A84_slps01144(TObj *, int, int);

#define MOVE o->h->raw += o->velH << 8; o->y.raw += o->velV << 8;
#define VMOVE o->y.raw += o->velV << 8; if (func_80042A84_slps01144(o, o->h->p.whole, (short)(o->y.p.whole + 8))) o->step++;
#define HMOVE o->h->raw += o->velH << 8; VMOVE
#define TAIL o->velH = 0; o->velV = 0; o->step++; playSFX(162);

void func_80117378_slps01144(TObj *o)
{
    switch (o->step) {
    case 0:
        if (o->b6a) {
            switch (o->b0c) {
            case 7:
            case 5:
                o->w74 = 0x300;
                o->w76 = 0x300;
                o->velX = 24;
                o->velY = 24;
                TAIL
                break;
            case 6:
                o->w74 = 0x300;
                o->w76 = 0x260;
                o->velX = 24;
                o->velY = 19;
                TAIL
                break;
            case 8:
                o->w74 = 0x400;
                o->w76 = 0x180;
                o->velX = 32;
                o->velY = 11;
                TAIL
                break;
            default:
                return;
            }
        }
        break;
    case 1:
        o->velH += o->velX;
        o->velV += o->velY;
        if (o->velH >= o->w74) {
            o->velH = o->w74;
            o->step++;
        }
        MOVE
        break;
    case 2:
        if (D_8009EE6B & 0x20) {
            if (o->b0c == 8) {
                if (o->a.p.whole < 2671) {
                    MOVE
                } else {
                    VMOVE
                }
            } else {
                HMOVE
            }
        } else {
            if (o->b0c == 8) {
                if (o->a.p.whole >= 2761) {
                    if (o->b6a) {
                        D_8009EAAE = 0;
                        D_8009E4E7 = 1;
                        D_8009E4E0 = 1;
                        D_8009E4E6 = 1;
                        D_8009E500[0]++;
                    }
                    o->step++;
                } else {
                    MOVE
                }
            } else {
                HMOVE
            }
        }
        break;
    case 3:
        if (o->visible == 0) {
            o->active = 2;
            o->step++;
            o->h->raw = o->d30;
            o->y.raw = o->d34;
            if (o->b0c == 8) D_8009E500[0] = 0;
        }
        break;
    case 4:
        if (o->visible == 0) {
            o->step = 0;
            o->active = 1;
        } else {
            o->visible = 0;
        }
        break;
    }
}
#endif
