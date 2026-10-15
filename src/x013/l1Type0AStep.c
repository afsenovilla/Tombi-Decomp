// FUNC 80111880 156 X013
// MATCHING 80111880 156
#include "TOBJ.H"
extern void l1Type0AStepSub0(TObj *);
extern void l1Type0AStepSub1(TObj *);

void l1Type0AStep(TObj *o)
{
    switch (o->subtype) {
    case 0:
        l1Type0AStepSub0(o);
        break;
    case 1:
        l1Type0AStepSub1(o);
        break;
    }
    if (o->w22 != 0) {
        o->w22--;
        if (o->w22 <= 0)
            o->active = 1;
    }
    if (o->b6a != 0) {
        o->b6a = 0;
        o->w22 = 3;
    }
}
