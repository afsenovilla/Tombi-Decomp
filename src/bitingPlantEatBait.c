// FUNC 801302a0 116 X000
// MATCHING 801302a0 116
#include "TOBJ.H"
extern void bitingPlantEatBaitStep(TObj *);

void bitingPlantEatBait(TObj *o)
{
    TObj *c;
    int i;
    if (o->subtype == 0) {
        i = 1;
        bitingPlantEatBaitStep(o);
        c = (TObj *)o->d94;
        do {
            bitingPlantEatBaitStep(c);
            c->state = o->state;
            c = (TObj *)c->d94;
            i++;
        } while (i < 3);
    }
}
