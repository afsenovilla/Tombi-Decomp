// FUNC 8012c718 364 X004
// MATCHING 8012c718 364
#include "gvars.h"
#include "TOBJ.H"

extern TObj PLAYER;
extern void *D_80135748, *D_80135750[];
extern int showMessageBoxDirect(int, int, void *);
extern void AnimLoadDuration(TObj *o);
extern int AnimAdvance(TObj *o);

void ownHouseThiefTalk(TObj *o)
{
    TObj *e;

    switch (o->state) {
    case 0:
        o->animFrame = 1;
        o->b68 = 0;
        o->state++;
        break;
    case 1:
        if (o->b68) {
            o->state++;
            GVARS.b3f = 1;
            GVARS.b42 = 1;
            PLAYER.b04 = 5;
            PLAYER.step = 0;
            PLAYER.state = 0;
            o->d90 = showMessageBoxDirect(5, 6, &o->a);
            o->wac = 2;
            o->animFrame = o->b68 & 1;
            o->anim = D_80135750[0];
            AnimLoadDuration(o);
        }
        break;
    case 2:
        AnimAdvance(o);
        e = (TObj *)o->d90;
        if (e->b04 == 2) {
            e->b04 = 3;
            GVARS.b3f = 0;
            GVARS.b42 = 0;
            PLAYER.b04 = 1;
            PLAYER.step = 0;
            PLAYER.state = 0;
            *(short *)((char *)o + 0xac) = 0;   /* raw store: the scalar anim load waits for it, not for the sb state */
            o->anim = D_80135748;
            o->state = 0;
        }
        break;
    }
}
