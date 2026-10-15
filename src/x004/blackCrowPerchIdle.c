// FUNC 80125420 308 X004
// MATCHING 80125420 308
#include "player.h"
#include "TOBJ.H"

extern void *D_801345DC[];
extern unsigned short D_80131128[];
extern void AnimLoadDuration(TObj *);
extern int Rand(void);

void blackCrowPerchIdle(TObj *o)
{
    unsigned short dy;

    switch (o->substep) {
    case 0:
        o->timer = 0x70;
        o->wb4 = 0;
        o->wac = 0xa;
        o->anim = D_801345DC[0];
        AnimLoadDuration(o);
        o->substep++;
    case 1:
        if (--o->timer) break;
        o->state = D_80131128[Rand() & 0xf];
        o->substep = 0;
        if (o->d->p.whole == (*(Fix16 **)&PLAYER.obj.d)->p.whole) {
            dy = (*(unsigned short*)&PLAYER.obj.y.p.whole) - o->y.p.whole + 0x30;
            if ((unsigned short)((*(Fix16 **)&PLAYER.obj.h)->p.whole - o->h->p.whole + 0x80) < 0x100 && dy < 0xf8) {
                o->state = 4;
                o->substep = 0;
            }
        }
        break;
    }
}
