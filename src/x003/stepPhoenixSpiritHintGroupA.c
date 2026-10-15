// FUNC 80129148 124 X003
// MATCHING 80129148 124
#include "TOBJ.H"

extern void phoenixSpiritHintEnterA(TObj *);
extern int AnimAdvance(TObj *);
extern void phoenixSpiritHintLeaveA(TObj *);

void stepPhoenixSpiritHintGroupA(TObj *o)
{
    switch (o->step) {
    case 0:
        phoenixSpiritHintEnterA(o);
        break;
    case 1:
        AnimAdvance(o);
        break;
    case 2:
        phoenixSpiritHintLeaveA(o);
        break;
    }
}
