// FUNC 80137968 404 X001
// MATCHING 80137968 404
#include "gvars.h"
#include "TOBJ.H"

extern TObj PLAYER;
extern void *D_8013E6DC;
extern unsigned char D_8009CEAF, D_8009D0B4, D_8009CE56;
extern int D_800A60C4[];
extern void AnimLoadDuration(TObj *o);
extern void FUN_800eea7c(TObj *, int, int);
extern void setEventStarted(int, int, int);

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
            GVARS.b3f = 1;
            GVARS.b42 = 1;
            GVARS.b3e = 1;
            o->animFrame = PLAYER.animFrame & 1;
            FUN_800eea7c(&PLAYER, 0xd, 0);
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
