// FUNC 8012c5ac 320 X010
// MATCHING 8012c5ac 320
#include "gvars.h"
#include "TOBJ.H"

extern TObj PLAYER;
extern void *D_801322F4[];
extern void ObjSetFacingToPlayer(TObj *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int showMessageBoxDirect(int, int, void *);

void nobleTreeGoldenItemsHint(TObj *o)
{
    TObj *q;

    switch (o->state) {
    case 0:
        o->state++;
        ObjSetFacingToPlayer(o);
        PLAYER.b04 = 5;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        PLAYER.step = 0;
        PLAYER.state = 0;
        o->wac = 5;
        o->anim = D_801322F4[0];
        AnimLoadDuration(o);
        o->d90 = showMessageBoxDirect(2, 0x28, &o->a);
        break;
    case 1:
        AnimAdvance(o);
        q = (TObj *)o->d90;
        if (q->b04 == 2) {
            q->b04 = 3;
            ((unsigned char*)&GVARS.b3f)[0] = 0;
            ((unsigned char*)&GVARS.b42)[0] = 0;
            PLAYER.b04 = 1;
            PLAYER.step = 0;
            PLAYER.state = 0;
            o->state++;
        }
        break;
    case 2:
        o->step = 0;
        o->state = 0;
        break;
    }
}
