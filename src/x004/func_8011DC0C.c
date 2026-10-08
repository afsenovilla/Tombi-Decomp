// FUNC 8011dc0c 596 X004
// MATCHING 8011dc0c 596
#include "TOBJ.H"
#include "raw7.h"

typedef struct { char p0[6]; unsigned char b6; char p7[2]; unsigned char b9; char pa[0x20 - 0xa]; short w20; } PL;
extern PL *D_8009C330;
extern unsigned short D_1F8001C8;
extern unsigned char D_801152E8[];
extern void FUN_800eeae4(TObj *, int, int);
extern void FUN_800eea7c(TObj *, int, int);
extern short FUN_8003fd78(TObj *, int, int);
extern int AnimAdvance(TObj *);

void func_8011DC0C(TObj *o)
{
    switch (o->state) {
    case 0:
        D_8009C330->b6 = o->active;
        D_8009C330->b9 = 0;
        o->velX = 0x5a;
        o->velY = 0x5a;
        o->active = 2;
        *(signed char *)&o->b0f = -8;
        U8(o, 0xa3) = 2;
        o->w74 = 6;
        o->d8c = 0;
        o->velY = 0;
        o->b9c = 0;
        o->b9d = 0;
        o->b9e = 0;
        o->b9f = 0;
        U8(o, 0xad) = 0;
        o->b69 = 0;
        o->w76 = 0;
        o->velV = 0x1e6;
        o->animFrame &= 1;
        FUN_800eeae4(o, 0x19, 1);
        o->state++;
    case 1:
        if (D_1F8001C8 & 1) {
            o->d->p.whole += 3;
        } else {
            o->d->p.whole -= 3;
        }
        o->velX -= 3;
        o->y.raw += o->velV << 8;
        o->velY -= 3;
        if (o->velX <= 0) {
            o->state = 9;
            FUN_800eea7c(o, 0x19, 2);
            if (FUN_8003fd78(o, o->w74, o->w76) && (U8(o, 0xa0) >> 4) == 4) {
                o->velX = 0x5a;
                o->velY = 0x5a;
                o->state = 1;
                FUN_800eea7c(o, 0x19, 1);
            }
        }
        break;
    case 9:
        if (AnimAdvance(o)) {
            *(signed char *)&o->b0f = -8;
            o->active = D_8009C330->b6;
            o->b9c = 0;
            o->ba5 = 0;
            U8(o, 0xac) = 0;
            o->wb2 = 0;
            o->velX = 0;
            o->velY = 0;
            D_8009C330->w20 = 0;
            o->d8c = D_801152E8[o->wb0];
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
