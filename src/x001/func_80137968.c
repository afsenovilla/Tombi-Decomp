// FUNC 80137968 404 X001
// MATCHING 80137968 404
#include "TOBJ.H"

extern TObj D_800A6038;
extern void *D_8013E6DC;
extern unsigned char D_8009CEAF, D_8009D0B4, D_8009CE56;
extern unsigned char D_8009C93F, D_8009C942, D_8009C93E;
extern int D_800A60C4[];
extern void AnimLoadDuration(TObj *o);
extern void FUN_800eea7c(TObj *, int, int);
extern void FUN_8005a8a8(int, int, int);

void func_80137968(TObj *o)
{
    switch (o->state) {
    case 0:
        *(signed char *)&o->b0f = -7;
        o->anim = D_8013E6DC;
        AnimLoadDuration(o);
        o->d8c = 0;
        o->state++;
        o->category |= 0x80;
    case 1:
        if (D_8009CEAF == 1 && !D_8009D0B4 && !D_8009CE56) {
            D_8009C93F = 1;
            D_8009C942 = 1;
            D_8009C93E = 1;
            o->animFrame = D_800A6038.animFrame & 1;
            FUN_800eea7c(&D_800A6038, 0xd, 0);
            D_800A60C4[0] = 0;
            D_800A6038.h->p.whole = o->h->p.whole;
            o->y.p.whole = D_800A6038.y.p.whole + 8;
            FUN_8005a8a8(0xb2, 0, 0);
            o->timer = 250;
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
