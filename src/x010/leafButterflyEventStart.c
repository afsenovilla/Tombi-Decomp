// FUNC 80123330 408 X010
// MATCHING 80123330 408
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"

extern TObj PLAYER;
extern void *D_80131DC0;
void AnimLoadDuration(TObj *o);
void playerSetAnim(TObj *o, short a, short c);
void setEventStarted(int a, int b, int c);

void leafButterflyEventStart(TObj *o)
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
        if (GCD.bceaf == 1 && GCD.bd0b4 == 0 && GCD.bce56 == 0) {
            GVARS.b3f = 1;
            GVARS.b42 = 1;
            GVARS.b3e = 1;
            o->animFrame = PLAYER.animFrame & 1;
            playerSetAnim(&PLAYER, 0xd, 0);
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
