// FUNC 80116f4c 48 X018
// MATCHING 80116f4c 48
#include "TOBJ.H"
extern void juiceManJoinFightTalk(TObj *o);
void juiceManJoinFightStep(TObj *o)
{
    if (o->step == 0) juiceManJoinFightTalk(o);
}
