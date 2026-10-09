// FUNC 80123330 408 X010
// MATCHING 80123330 408
#include "TOBJ.H"

extern TObj D_800A6038;
extern void *D_80131DC0;
extern unsigned char D_8009CEAF;
extern unsigned char D_8009D0B4;
extern unsigned char D_8009CE56;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009C93E;
void AnimLoadDuration(TObj *o);
void FUN_800eea7c(TObj *o, short a, short c);
void FUN_8005a8a8(int a, int b, int c);

void func_80123330(TObj *o)
{
    switch (o->state) {
    case 0:
        *(signed char *)&o->b0f = -7;
        o->anim = D_80131DC0;
        AnimLoadDuration(o);
        o->state++;
        o->d8c = 0;
        o->category |= 0x80;
        break;
    case 1:
        if (D_8009CEAF == 1 && D_8009D0B4 == 0 && D_8009CE56 == 0) {
            D_8009C93F = 1;
            D_8009C942 = 1;
            D_8009C93E = 1;
            o->animFrame = D_800A6038.animFrame & 1;
            FUN_800eea7c(&D_800A6038, 0xd, 0);
            D_800A6038.d8c = 0;
            D_800A6038.h->p.whole = o->h->p.whole;
            o->y.p.whole = D_800A6038.y.p.whole + 8;
            FUN_8005a8a8(0xb2, 0, 0);
            o->timer = 0xfa;
            o->state++;
        }
        break;
    case 2:
        if (--o->timer <= 0) {
            D_8009C93F = 0;
            D_8009C942 = 0;
            D_8009C93E = 0;
            o->state = 1;
        }
        break;
    }
}
