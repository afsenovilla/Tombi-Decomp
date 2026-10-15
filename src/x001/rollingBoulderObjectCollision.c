// FUNC 80125fbc 72 X001
// MATCHING 80125fbc 72
#include "TOBJ.H"
extern short boxesOverlapSide(TObj *, TObj *);
extern void resolveHitResultA(TObj *, TObj *);

void rollingBoulderObjectCollision(TObj *a, TObj *b)
{
    if (boxesOverlapSide(a, b) >= 0) resolveHitResultA(a, b);
}
