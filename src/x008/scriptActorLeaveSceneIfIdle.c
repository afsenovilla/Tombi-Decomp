// FUNC 8011465c 48 X008
// MATCHING 8011465c 48
#include "TOBJ.H"
extern void scriptActorLeaveScene(TObj *o);
void scriptActorLeaveSceneIfIdle(TObj *o)
{
    if (o->step == 0) scriptActorLeaveScene(o);
}
