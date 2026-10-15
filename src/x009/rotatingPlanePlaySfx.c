// FUNC 801178b0 124 X009
// MATCHING 801178b0 124
#include "TOBJ.H"

extern void rotatingPlaneLevelStep(TObj *);
extern void rotatingPlaneRaisedStep(TObj *);
extern void rotatingPlaneLoweredStep(TObj *);

void rotatingPlanePlaySfx(TObj *o)
{
    switch (o->step) {
    case 0:
        rotatingPlaneLevelStep(o);
        break;
    case 1:
        rotatingPlaneRaisedStep(o);
        break;
    case 2:
        rotatingPlaneLoweredStep(o);
        break;
    }
}
