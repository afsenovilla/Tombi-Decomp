// FUNC 8011b170 780 X014
// MATCHING 8011b170 780
#include "TOBJ.H"
#include "raw7.h"

typedef struct {
    unsigned char b0;
    unsigned char p1[7];
    unsigned char b8;
} P8011B170;

extern P8011B170 *D_8009C330;
extern TObj *D_8009F0EC;
extern short D_8009C944;
extern short D_8009C946[];
extern char D_80077D24[];
extern void FUN_800eee90(TObj *);
extern void FUN_800fc104(TObj *);
extern void FUN_800eeb5c(TObj *, int);
extern void FUN_8001e560(int, int);
extern void FUN_8001e4f0(int);
extern int AnimAdvance(TObj *);
extern void FUN_8001fa88(TObj *, int);
extern int FUN_8003facc(TObj *);
extern short FUN_8003fd78(TObj *, int, int);

void func_8011B170(TObj *o)
{
    switch (o->state) {
    case 0:
        o->velY = -0x500;
        o->animFrame = 1 - (o->animFrame & 1);
        o->state++;
    case 1:
        o->d8c = 0;
        if (o->b9e != 0)
            D_8009F0EC->b6a = 0;
        o->b9e = 0;
        U8(o, 0xaa) = 0;
        o->ba7 = 0;
        U8(o, 0xc3) = 0;
        o->wb0 = 0;
        D_8009C330->b8 = 0;
        FUN_800eee90(o);
        *(signed char *)&o->b0f = -8;
        FUN_800fc104(o);
        o->ba4 = 0;
        o->b69 = 0;
        o->velV = 0;
        o->movetab = D_80077D24;
        FUN_800eeb5c(o, 0x39);
        o->y.raw += o->velY << 8;
        FUN_8001e560(0x23, 0x24);
        FUN_8001e4f0(0x1f);
        o->state++;
        break;
    case 2:
        AnimAdvance(o);
        if (o->b69 == 0 && !(o->animFrame & 2))
            FUN_8001fa88(o, o->animFrame ^ 1);
        o->h->raw += D_8009C944 << 8;
        o->y.raw += D_8009C946[0] << 8;
        o->velY += 0x40;
        o->y.raw += o->velY << 8;
        if (o->velY > 0) {
            FUN_800eeb5c(o, 0x3a);
            o->b9c = 2;
            o->state++;
        }
        if (FUN_8003facc(o)) {
            FUN_800eeb5c(o, 0x3a);
            o->b9c = 2;
            o->velY = 0;
            o->state = 3;
        }
        break;
    case 3:
        AnimAdvance(o);
        if (o->b69 == 0 && !(o->animFrame & 2))
            FUN_8001fa88(o, o->animFrame ^ 1);
        o->h->raw += D_8009C944 << 8;
        o->y.raw += D_8009C946[0] << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (o->velY > 0x680)
            o->velY = 0x680;
        if (o->b69 || FUN_8003fd78(o, 0, 0)) {
            FUN_800eee90(o);
            FUN_800eeb5c(o, 0);
            o->b04 = 1;
            o->step = 0;
            o->state = 0;
            o->substep = 0;
        }
        break;
    }
}
