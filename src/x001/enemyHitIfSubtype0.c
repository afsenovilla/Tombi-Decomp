// FUNC 8012593c 88 X001
// MATCHING 8012593c 88
#include "TOBJ.H"
extern short boxesOverlapSide(TObj *, TObj *);
extern int resolveHitResultA(TObj *, TObj *);

void func_8012593C(TObj *o, TObj *e)
{
    if (e->subtype == 0 && boxesOverlapSide(o, e) >= 0) resolveHitResultA(o, e);
}
