// FUNC 801331fc 196 X003
// MATCHING 801331fc 196
#include "TOBJ.H"
#include "gamestate.h"
extern void func_80131E44(TObj *);
extern void func_80132214(TObj *);
extern void phoenixSpiritHintTalkB(TObj *);
extern void func_80132960(TObj *);
extern void flameBlockedNpcTalk(TObj *);

void func_801331FC(TObj *o)
{
    if (GAME.selectedSection == 0) {
        switch (o->step) {
        case 0:
            func_80131E44(o);
            break;
        case 1:
            func_80132214(o);
            break;
        case 2:
            phoenixSpiritHintTalkB(o);
            break;
        }
    } else if (GAME.selectedSection == 1) {
        if (o->step == 0) func_80132960(o);
    } else {
        flameBlockedNpcTalk(o);
    }
}
