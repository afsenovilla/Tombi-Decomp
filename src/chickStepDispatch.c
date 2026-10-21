// FUNC 8012de8c 80 X000
// MATCHING 8012de8c 80
#include "TOBJ.H"
extern void chickDropToGround(TObj *);
extern void chickWander(TObj *);

void chickStepDispatch(TObj *o)
{
    switch (o->step) {
    case 0:
        chickDropToGround(o);
        break;
    case 1:
        chickWander(o);
        break;
    }
}
