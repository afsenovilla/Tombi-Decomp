// FUNC 801331fc 196 X003
// MATCHING 801331fc 196
#include "TOBJ.H"
#include "gamestate.h"
extern void soakedPhoenixGuideIntroScene(TObj *);
extern void soakedPhoenixGuideTalk(TObj *);
extern void phoenixSpiritHintTalkB(TObj *);
extern void soakedPhoenixGuideSection1Scene(TObj *);
extern void flameBlockedNpcTalk(TObj *);

void stepSoakedPhoenixGuideTalk(TObj *o)
{
    if (GAME.selectedSection == 0) {
        switch (o->step) {
        case 0:
            soakedPhoenixGuideIntroScene(o);
            break;
        case 1:
            soakedPhoenixGuideTalk(o);
            break;
        case 2:
            phoenixSpiritHintTalkB(o);
            break;
        }
    } else if (GAME.selectedSection == 1) {
        if (o->step == 0) soakedPhoenixGuideSection1Scene(o);
    } else {
        flameBlockedNpcTalk(o);
    }
}
