// FUNC 801168c0 64 X014
// MATCHING 801168c0 64
#include "TOBJ.H"
extern void stepScrollReturnY(TObj *);
extern void FUN_80028548(TObj *);
extern void FUN_80027ed8(TObj *);
extern void FUN_8002809c(TObj *);

void func_801168C0(TObj *o)
{
    stepScrollReturnY(o);
    FUN_80028548(o);
    FUN_80027ed8(o);
    FUN_8002809c(o);
}
