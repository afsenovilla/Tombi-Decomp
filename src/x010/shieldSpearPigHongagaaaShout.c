// FUNC 8012cce8 408 X010
// MATCHING 8012cce8 408
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
extern void *D_8013238C;
extern void *D_80132388;
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern int Rand(void);
extern TObj *showMessageBoxDirect(int, int, Fix16 *);

/* do-while(0) macro: its loop notes end the sched block, so the a0 copy stays after the stores */
#define SET_ANIM(o, a) do { (o)->anim = (a); AnimLoadDuration(o); } while (0)

void shieldSpearPigHongagaaaShout(TObj *o)
{
    TObj *p;

    switch (o->state) {
    case 0:
        o->b68 = 0;
        o->state++;
        break;
    case 1:
        if (o->b68) {
            o->state++;
            ((unsigned char*)&GVARS.b3f)[0] = 1;
            ((unsigned char*)&GVARS.b42)[0] = 1;
            ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
            ((unsigned char*)&PLAYER.obj.step)[0] = 0;
            ((unsigned char*)&PLAYER.obj.state)[0] = 0;
            *(TObj **)&o->d90 = showMessageBoxDirect(2, 3, &o->a);
            o->wac = 0x1f;
            o->animFrame = o->b68 & 1;
            SET_ANIM(o, D_8013238C);
        }
        break;
    case 2:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
            ((unsigned char*)&GVARS.b3f)[0] = 0;
            ((unsigned char*)&GVARS.b42)[0] = 0;
            ((unsigned char*)&PLAYER.obj.step)[0] = 0;
            ((unsigned char*)&PLAYER.obj.state)[0] = 0;
            o->wac = 0x1e;
            o->anim = D_80132388;
            o->state++;
        }
        break;
    case 3:
        o->b6b = Rand() & 1;
        o->state = 0;
        break;
    }
}
