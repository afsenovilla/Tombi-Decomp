// FUNC 8011baa0 524 X010
// MATCHING 8011baa0 524
#include "TOBJ.H"
#include "raw7.h"

extern TObj *D_8009C330;
extern short D_8009C944, D_8009C946[];
extern int D_8009D2E8;
extern void FUN_800ee428(TObj *);
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void func_80110FDC(TObj *);

void func_8011BAA0(TObj *o)
{
    TObj *pl;
    int v;

    switch (o->state) {
    case 0:
        FUN_800ee428(o);
        o->b9d = 1;
        U8(o, 0xc8) = 0;
        U8(o, 0xa0) = 0;
        U8(o, 0xa1) = 0;
        o->wb0 = 0;
        o->wb6 = 0;
        D_8009C330->active = 2;
        D_8009C330->animFrame = 0xffff;
        U16(D_8009C330, 0x28) = 0xffff;
        U16(D_8009C330, 0x2a) = 0xffff;
        o->state = 2;
        break;
    case 2:
        o->h->raw += D_8009C944 << 8;
        o->y.raw += D_8009C946[0] << 8;
        func_80110FDC(o);
        o->h->raw += o->velX << 8;
        o->y.raw += o->velY << 8;
        o->velY += 0x40;
        if (U8(o, 0xac) == 2) {
            D_8009D2E8 = S32(o, 0xe4);
            U8(D_8009C330, 8) = 0;
            o->ba5 = 0;
            o->step = 0xe;
            o->state = 0;
            break;
        }
        if (o->velY > 0) {
            FUN_800ee428(o);
            U8(o, 0xac) = 1;
            o->b9c = 2;
            o->step = 4;
            o->state = 3;
        }
        pl = D_8009C330;
        if (pl->active == 0) {
            if (U8(o, 0xc6) == 0) {
                U8(o, 0xac) = 0;
                PlayerSetAnimIfChanged(o, 4);
                o->b9d = 0;
                o->b9c = 1;
                o->d84 = 0;
                if (o->animFrame & 1)
                    v = 0xf0;
                else
                    v = 0x10;
                o->d88 = v;
                o->d8c = 0;
                o->step = 2;
                o->state = 2;
            }
        } else if (U8(o, 0xc8)) {
            U8(pl, 8) = 0;
            U8(o, 0xac) = 0;
            o->ba7 = 0;
            o->b9c = 0;
            o->step = 0x32;
            o->state = 0;
        }
        break;
    }
}
