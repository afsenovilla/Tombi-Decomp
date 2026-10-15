// FUNC 80129868 452 X003
// MATCHING 80129868 452
#include "gv2.h"
#include "player.h"
#include "TOBJ.H"

extern void phoenixSpiritHintEnterA(TObj *);
extern void phoenixSpiritHintLeaveA(TObj *);
extern void phoenixSpiritHintEnterB(TObj *);
extern void phoenixSpiritHintLeaveB(TObj *);
extern void phoenixSpiritHintWalkC(TObj *);
extern void phoenixSpiritHintLeaveC(TObj *);
extern void phoenixSpiritHintTalk(TObj *);
extern int FUN_8001fec0(TObj *);

void stepPhoenixSpiritHintTalk(TObj *o)
{
    switch ((unsigned short)o->wba) {
    case 0:
        switch (o->step) {
        case 0:
            phoenixSpiritHintEnterA(o);
            break;
        case 1:
            goto anim;
        case 2:
            phoenixSpiritHintLeaveA(o);
            break;
        }
        break;
    case 1:
        switch (o->step) {
        case 0:
            phoenixSpiritHintEnterB(o);
            break;
        case 1:
            goto anim;
        case 2:
            phoenixSpiritHintLeaveB(o);
            break;
        }
        break;
    case 2:
        switch (o->step) {
        case 0:
            goto anim;
        case 1:
            phoenixSpiritHintWalkC(o);
            break;
        case 2:
            phoenixSpiritHintLeaveC(o);
            break;
        }
        break;
    case 3:
        switch (o->step) {
        case 0:
            goto anim;
        case 1:
            phoenixSpiritHintTalk(o);
            break;
        }
        break;
    case 4:
        if (!(GV2.bc3 & 4)) {
            if ((*(short **)&PLAYER.obj.h)[1] > 0xa0) (*(short **)&PLAYER.obj.h)[1] = 0xa0;
        }
    anim:
        FUN_8001fec0(o);
        break;
    }
}
