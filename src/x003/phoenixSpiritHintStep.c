// FUNC 80129818 80 X003
// MATCHING 80129818 80
#include "TOBJ.H"
extern void FUN_8001fec0(TObj *);
extern void phoenixSpiritHintTalk(TObj *);

void phoenixSpiritHintStep(TObj *o)
{
    switch (o->step) {
    case 0:
        FUN_8001fec0(o);
        break;
    case 1:
        phoenixSpiritHintTalk(o);
        break;
    }
}
