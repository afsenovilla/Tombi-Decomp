// FUNC 80129250 336 X004
// MATCHING 80129250 336
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
#include "gamestate.h"
extern void mermaidNecklaceTalk(TObj *);
extern void mermaidDivingOfferTalk(TObj *);
extern void mermaidDivingLesson(TObj *);
extern void FUN_8001fec0(TObj *);

void stepMermaidTalk(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GCD.bce22 == 0) break;
        if (GVARS.b40 != 0 && GVARS.b41 == 0x94) {
            o->step = 2;
            o->state = 0;
        }
        if (PLAYER.obj.y.p.whole >= -0xa8 && GAME.displayExpBar != 1 && GCD.bce3d != 0 && GCD.bce3d != 0xff) {
            o->step = 3;
            o->state = 0;
        }
        if (o->b68 != 0) {
            o->animFrame = o->b68 & 1;
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        mermaidNecklaceTalk(o);
        break;
    case 2:
        mermaidDivingOfferTalk(o);
        break;
    case 3:
        mermaidDivingLesson(o);
        break;
    }
    FUN_8001fec0(o);
}
