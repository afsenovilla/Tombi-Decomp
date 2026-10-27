// FUNC 8002f060 268 MAIN0
// MATCHING 8002f060 268
#include "TOBJ.H"
#include "gamestate.h"
#include "player.h"
typedef struct { void **a; int p[2]; } A12;
extern A12 PLAYER_EFFECT_4E_ANIM_TABLE[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);

void playerEffect4EStepE(TObj *o)
{
    switch (o->state) {
    case 0:
        o->anim = PLAYER_EFFECT_4E_ANIM_TABLE[o->subtype].a[o->b0c];
        AnimLoadDuration(o);
        o->visible = PLAYER.obj.visible;
        o->b0f = PLAYER.obj.b0f - 1;
        o->b6b = 0x7f;
        if (GAME.displayExpBar == 3) {
            o->b6b = 0x40;
        }
        o->state++;
        break;
    case 1:
        if (AnimAdvance(o)) {
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
        }
        if (o->visible == 0) {
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
