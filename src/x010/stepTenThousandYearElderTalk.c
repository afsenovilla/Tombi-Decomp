// FUNC 80126ea0 168 X010
// MATCHING 80126ea0 168
#include "TOBJ.H"

extern void selectTenThousandYearElderTalkStep(TObj *);
extern void tenThousandYearElderTalk(TObj *);
extern void tenThousandYearElderBeadsTalk(TObj *);
extern void powerFishCollectorTalk(TObj *);
extern int AnimAdvance(TObj *);

void stepTenThousandYearElderTalk(TObj *o)
{
    switch (o->step) {
    case 0:
        selectTenThousandYearElderTalkStep(o);
        break;
    case 1:
        tenThousandYearElderTalk(o);
        break;
    case 2:
        tenThousandYearElderBeadsTalk(o);
        break;
    case 3:
        powerFishCollectorTalk(o);
        break;
    }
    AnimAdvance(o);
}
