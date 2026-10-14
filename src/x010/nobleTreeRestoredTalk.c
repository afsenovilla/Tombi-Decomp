// FUNC 8012bf24 440 X010
// MATCHING 8012bf24 440
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"

extern TObj PLAYER;
extern void *D_801322F4[];
extern void ObjSetFacingToPlayer(TObj *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int showMessageBoxDirect(int, int, void *);
extern void setEventStarted(int, int, int);

void nobleTreeRestoredTalk(TObj *o)
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
        o->d90 = showMessageBoxDirect(2, 8, &o->a);
        break;
    case 3:
        o->state++;
        o->d90 = showMessageBoxDirect(2, 9, &o->a);
        break;
    case 1:
    case 4:
        AnimAdvance(o);
        {
            TObj *p = (TObj *)o->d90;
            if (p->b04 == 2) {
                p->b04 = 3;
                o->state++;
            }
        }
        break;
    case 2:
    case 5:
        o->state++;
        break;
    case 6:
        o->state++;
        o->d90 = showMessageBoxDirect(2, 10, &o->a);
        break;
    case 7:
        AnimAdvance(o);
        q = (TObj *)o->d90;
        if (q->b04 == 2) {
            q->b04 = 3;
            o->state++;
            ((unsigned char*)&GVARS.b3f)[0] = 0;
            ((unsigned char*)&GVARS.b42)[0] = 0;
            PLAYER.b04 = 1;
            PLAYER.step = 0;
            PLAYER.state = 0;
            setEventStarted(0x3e, 0, 1);
        }
        break;
    case 8:
        o->step = 0;
        o->state = 0;
        GCD.bd07a = 2;
        break;
    }
}
