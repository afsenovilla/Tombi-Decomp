// FUNC 801258e4 88 X001
// MATCHING 801258e4 88
#include "TOBJ.H"
extern short boxesOverlapSide(TObj *, TObj *);
extern int resolveHitResultA(TObj *, TObj *);

void func_801258E4(TObj *o, TObj *e)
{
    if (e->subtype == 0 && boxesOverlapSide(o, e) >= 0) resolveHitResultA(o, e);
}
