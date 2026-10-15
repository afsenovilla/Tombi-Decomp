// FUNC 801291c4 124 X003
// MATCHING 801291c4 124
#include "TOBJ.H"

extern void phoenixSpiritHintEnterB(TObj *);
extern int AnimAdvance(TObj *);
extern void phoenixSpiritHintLeaveB(TObj *);

void stepPhoenixSpiritHintGroupB(TObj *o)
{
    switch (o->step) {
    case 0:
        phoenixSpiritHintEnterB(o);
        break;
    case 1:
        AnimAdvance(o);
        break;
    case 2:
        phoenixSpiritHintLeaveB(o);
        break;
    }
}
