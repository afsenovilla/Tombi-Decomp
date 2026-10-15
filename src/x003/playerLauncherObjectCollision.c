// FUNC 801202e4 72 X003
// MATCHING 801202e4 72
#include "TOBJ.H"
extern short boxesOverlapSide(TObj *, TObj *);
extern void resolveHitResultA(TObj *, TObj *);

void playerLauncherObjectCollision(TObj *a, TObj *b)
{
    if (boxesOverlapSide(a, b) >= 0) resolveHitResultA(a, b);
}
