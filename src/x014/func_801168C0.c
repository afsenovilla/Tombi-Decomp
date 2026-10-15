// FUNC 801168c0 64 X014
// MATCHING 801168c0 64
#include "TOBJ.H"
extern void stepScrollReturnY(TObj *);
extern void FUN_80028548(TObj *);
extern void cameraFollowTargetY(TObj *);
extern void cameraFollowTargetYB(TObj *);

void func_801168C0(TObj *o)
{
    stepScrollReturnY(o);
    FUN_80028548(o);
    cameraFollowTargetY(o);
    cameraFollowTargetYB(o);
}
