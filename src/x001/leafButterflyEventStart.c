// FUNC 80137968 404 X001
// MATCHING 80137968 404
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"

extern TObj PLAYER;
extern void *D_8013E6DC;
extern int D_800A60C4[];
extern void AnimLoadDuration(TObj *o);
extern void playerSetAnim(TObj *, int, int);
extern void setEventStarted(int, int, int);

void leafButterflyEventStart(TObj *o)
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
        if (GCD.bceaf == 1 && !GCD.bd0b4 && !GCD.bce56) {
            GVARS.b3f = 1;
            GVARS.b42 = 1;
            GVARS.b3e = 1;
            o->animFrame = PLAYER.animFrame & 1;
            playerSetAnim(&PLAYER, 0xd, 0);
            D_800A60C4[0] = 0;
            PLAYER.h->p.whole = o->h->p.whole;
            o->y.p.whole = PLAYER.y.p.whole + 8;
            setEventStarted(0xb2, 0, 0);
            o->timer = 250;
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
