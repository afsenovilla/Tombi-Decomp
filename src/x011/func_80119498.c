// FUNC 80119498 740 X011
// MATCHING 80119498 740
#include "gcd.h"
#include "TOBJ.H"

extern int AnimAdvance(TObj *o);
extern void goGoCarMechanicTalk(TObj *o);
extern void goGoCarFuelTalk(TObj *o);
extern void goGoCarWineReceivedTalk(TObj *o);
extern void bonsugeeResearcherTalk(TObj *o);
extern void bonsugeeResearcherRepeatTalk(TObj *o);
extern void factoryBossTalk(TObj *o);
extern void func_80118328(TObj *o);
extern void factoryBossCoconutBombTalk(TObj *o);
extern void factoryBossNoBombHereTalk(TObj *o);
extern void factoryBossDontUseBombTalk(TObj *o);
extern void factoryBossDoorAskTalk(TObj *o);
extern void powerPlantThanksTalk(TObj *o);
extern void powerPlantElevatorHintTalk(TObj *o);
extern void func_8011921C(TObj *o);

void func_80119498(TObj *o)
{
    switch (o->subtype) {
    case 0:
        switch (o->step) {
        case 0:
            if (GCD.bcdf1 != 0xff || GCD.bce0c == 0xff) {
                o->b04 = 2;
                break;
            }
            if (GCD.bce0c) o->step = 2;
            else o->step = 1;
            o->state = 0;
            break;
        case 1:
            goGoCarMechanicTalk(o);
            break;
        case 2:
            goGoCarFuelTalk(o);
            break;
        case 3:
            goGoCarWineReceivedTalk(o);
            break;
        }
        break;
    case 1:
        switch (o->step) {
        case 0:
            if (GCD.bcde5) o->step = 2;
            else o->step = 1;
            o->state = 0;
            break;
        case 1:
            bonsugeeResearcherTalk(o);
            break;
        case 2:
            bonsugeeResearcherRepeatTalk(o);
            break;
        }
        break;
    case 2:
        switch (o->step) {
        case 0:
            switch (GCD.bce0a) {
            case 0:
                o->step = 1;
                break;
            case 0xff:
                if (GCD.bcdf1 != 0xff) o->step = 6;
                else if (GCD.bcdf3) o->step = 8;
                else o->step = 7;
                break;
            default:
                switch (GCD.bce0d) {
                case 0:
                    o->step = 2;
                    break;
                case 0xff:
                    o->step = 4;
                    break;
                default:
                    o->step = 3;
                    break;
                }
                break;
            }
            o->state = 0;
            break;
        case 1: factoryBossTalk(o); break;
        case 2: func_80118328(o); break;
        case 3: factoryBossCoconutBombTalk(o); break;
        case 4: factoryBossNoBombHereTalk(o); break;
        case 5: factoryBossDontUseBombTalk(o); break;
        case 6: factoryBossDoorAskTalk(o); break;
        case 7: powerPlantThanksTalk(o); break;
        case 8: powerPlantElevatorHintTalk(o); break;
        }
        break;
    case 3:
        func_8011921C(o);
        break;
    }
    AnimAdvance(o);
}
