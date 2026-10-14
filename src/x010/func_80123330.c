// FUNC 80123330 408 X010
// MATCHING 80123330 408
#include "gvars.h"
#include "TOBJ.H"

extern TObj PLAYER;
extern void *D_80131DC0;
extern unsigned char D_8009CEAF;
extern unsigned char D_8009D0B4;
extern unsigned char D_8009CE56;
void AnimLoadDuration(TObj *o);
void FUN_800eea7c(TObj *o, short a, short c);
void setEventStarted(int a, int b, int c);

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
            GVARS.b3f = 1;
            GVARS.b42 = 1;
            GVARS.b3e = 1;
            o->animFrame = PLAYER.animFrame & 1;
            FUN_800eea7c(&PLAYER, 0xd, 0);
            PLAYER.d8c = 0;
            PLAYER.h->p.whole = o->h->p.whole;
            o->y.p.whole = PLAYER.y.p.whole + 8;
            setEventStarted(0xb2, 0, 0);
            o->timer = 0xfa;
            o->state++;
        }
        break;
    case 2:
        if (--o->timer <= 0) {
            GVARS.b3f = 0;
            GVARS.b42 = 0;
            GVARS.b3e = 0;
            o->state = 1;
        }
        break;
    }
}
