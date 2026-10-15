// FUNC 8011f5b4 72 X010
// MATCHING 8011f5b4 72
#include "TOBJ.H"
extern short boxesOverlapSide(TObj *, TObj *);
extern void resolveHitResultA(TObj *, TObj *);

void solidSceneryObjectCollision(TObj *a, TObj *b)
{
    if (boxesOverlapSide(a, b) >= 0) resolveHitResultA(a, b);
}
