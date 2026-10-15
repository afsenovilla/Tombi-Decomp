// FUNC 80119bf4 760 X001
// MATCHING 80119bf4 760
// VERSION SLPS_01144 x01 80118BCC 760
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("x01/nonmatchings/x01", func_80118BCC_slps01144);
#else
#include "spad.h"
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
extern void *D_8013E540[];
extern unsigned short D_1F80016E[];
extern unsigned short D_1F800172[];
typedef struct { short x; } SX;
extern SX D_1F80016A;
extern short D_8007A5F0[];
extern int FUN_800202b4(TObj *);
extern void FUN_800187e4(TObj *);
extern void FUN_80026e0c(int, int);
extern void spawnFlowerTearsSparkle(TObj *);

void updateFlowerTearsThrow(TObj *o)
{

    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 10;
        o->b0d = 0;
        o->d3c = ((int*)&SPAD.i2d4)[0];
        o->anim = D_8013E540[0];
        o->d30 = o->h->p.whole;
        o->d34 = o->y.p.whole;
        o->d38 = o->d->p.whole;
        o->b0a = 2;
        o->d8c = 0;
        *(signed char *)&o->b0f = -10;
        break;
    case 1:
        switch (o->step) {
        case 0:
            if (GVARS.b40 && GVARS.b41 == 0xd) {
                o->h->p.whole = D_1F80016A.x;
                o->y.p.whole = D_1F80016E[0] - 0x20;
                o->d->p.whole = D_1F800172[0];
                o->step++;
            }
            break;
        case 1:
            FUN_800202b4(o);
            o->timer = 0x3c;
            o->velV = 0x180;
            o->step++;
            o->velH = ((0xb5e - D_1F80016A.x) << 8) / 32;
            if (D_1F80016A.x < 0xb5e)
                o->animFrame = 0;
            else
                o->animFrame = 1;
            o->d88 = 0;
            break;
        case 2:
            FUN_800202b4(o);
            if (o->d88 < 0x41) {
                o->h->raw += o->velH << 8;
                o->y.raw -= (D_8007A5F0[o->d88 & 0xff] * o->velV) >> 4;
                o->d88 += 2;
            }
            if (o->animFrame)
                o->d8c += 4;
            else
                o->d8c -= 4;
            spawnFlowerTearsSparkle(o);
            if (--o->timer == -1)
                o->step++;
            break;
        case 3:
            GCD.bd081 = 1;
            FUN_80026e0c(0xd, 1);
            o->b04 = 3;
            break;
        }
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
#endif
