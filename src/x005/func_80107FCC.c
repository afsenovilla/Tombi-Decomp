// FUNC 80107fcc 412 X005
// MATCHING 80107fcc 412
#include "TOBJ.H"
#include "raw7.h"
extern TObj *D_8009C330;
extern unsigned char D_8009CDA2;
extern unsigned short D_1f8001c8;
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void AnimAdvance(TObj *);
void func_80107FCC(TObj *o)
{
    unsigned short f;
    switch (o->state) {
    case 0:
        U8(D_8009C330, 8) = o->active;
        f = o->animFrame;
        o->velX = 0x78;
        o->active = 2;
        o->d8c = 0;
        o->velY = 0;
        o->b9c = 0;
        o->b9d = 0;
        o->b9e = 0;
        o->b9f = 0;
        U8(o, 0xad) = 0;
        o->b69 = 0;
        S8(o, 0xf) = -20;
        U8(o, 0xa2) = 2;
        o->animFrame = f & 1;
        PlayerSetAnimIfChanged(o, 0x2b);
        o->state++;
    case 1:
        AnimAdvance(o);
        if (D_8009CDA2 == 0) {
            o->state++;
        }
        break;
    case 2:
        AnimAdvance(o);
        if (D_1f8001c8 & 1) {
            o->d->p.whole = o->d->p.whole - 5;
        } else {
            o->d->p.whole = o->d->p.whole + 5;
        }
        if ((o->velX -= 5) == 0) {
            S8(o, 0xf) = -8;
            U8(o, 0) = U8(D_8009C330, 8);
            o->b9c = 0;
            o->wb2 = 0;
            o->velX = 0;
            o->velY = 0;
            D_8009C330->timer = 0;
        }
        break;
    }
}
