// FUNC 801295b0 124 X003
// MATCHING 801295b0 124
#include "TOBJ.H"

extern int AnimAdvance(TObj *);
extern void phoenixSpiritHintWalkC(TObj *);
extern void phoenixSpiritHintLeaveC(TObj *);

void stepPhoenixSpiritHintGroupC(TObj *o)
{
    switch (o->step) {
    case 0:
        AnimAdvance(o);
        break;
    case 1:
        phoenixSpiritHintWalkC(o);
        break;
    case 2:
        phoenixSpiritHintLeaveC(o);
        break;
    }
}
