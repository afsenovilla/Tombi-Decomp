// FUNC 80117c84 536 X017
// MATCHING 80117c84 536
#include "gcd.h"
#include "gv2.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
extern void ironCastleMachineRoomTalk(TObj *);
extern void waterfallElderHintTalk(TObj *);
extern void waterfallHintNpcWalkScene(TObj *);
extern void pigGateFavorTalk(TObj *);
extern void setEventComplete(int, int);

void stepWaterfallHintNpcTalk(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GVARS.useItemPending != 0) {
            if (GVARS.useItemId == 0x3c && GCD.bce0a == 1) {
                o->step = 3;
                o->state = 0;
            }
            GVARS.useItemPending = 0;
        } else {
            if (o->b68 != 0 && (unsigned short)o->wba == 1) {
                o->step = 5;
                o->state = 0;
            }
            if (GCD.bce0a != 0xff) {
                if (GCD.bcfd6 == 1) o->step = 4;
                else o->step = 1;
                o->state = 0;
            }
        }
        break;
    case 1:
        ironCastleMachineRoomTalk(o);
        break;
    case 2:
        waterfallElderHintTalk(o);
        break;
    case 3:
        waterfallHintNpcWalkScene(o);
        break;
    case 4:
        switch (o->state) {
        case 0:
            if (GVARS.playerLanded != 0) {
                GVARS.b42 = 1;
                GVARS.b3f = 1;
                GV2.bb0 = 2;
                PLAYER.obj.b04 = 5;
                PLAYER.obj.step = 0;
                PLAYER.obj.state = 0;
                setEventComplete(0x66, 0);
                o->timer = 300;
                o->state++;
            }
            break;
        case 1:
            if (--o->timer <= 0) {
                GCD.bcfd6 = 1;
                PLAYER.obj.b04 = 1;
                PLAYER.obj.step = 0x20;
                PLAYER.obj.state = 0;
                o->state++;
            }
            break;
        }
        break;
    case 5:
        pigGateFavorTalk(o);
        break;
    }
}
