// FUNC 801298dc 56 X004
// MATCHING 801298dc 56
#include "TOBJ.H"
extern void scaredGoldVolunteerTalk(TObj *);

void stepScaredGoldVolunteerTalk(TObj *o)
{
    switch (o->step) {
    case 0:
        scaredGoldVolunteerTalk(o);
        break;
    case 1:
        scaredGoldVolunteerTalk(o);
        break;
    }
}
