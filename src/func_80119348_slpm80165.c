// VERSION SLPM_80165 x00 80119348 60
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 8011465C (scriptActorLeaveSceneIfIdle.c): runs every 8th tick of +0x20 instead of on step 0.
#include "TOBJ.H"
extern void FUN_801142c0(TObj *o);
void scriptActorLeaveSceneIfIdle(TObj *o)
{
    if ((++o->timer & 7) == 0) FUN_801142c0(o);
}
#endif
