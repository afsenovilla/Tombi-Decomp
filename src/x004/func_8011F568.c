// FUNC 8011f568 80 X004
// MATCHING 8011f568 80
#include "TOBJ.H"

extern short D_1F80019E;
extern short boxesOverlapSide(TObj *, TObj *);
extern void resolveHitResultA(TObj *, TObj *);

void func_8011F568(TObj *o, TObj *e)
{
    if (boxesOverlapSide(o, e) >= 0) {
        resolveHitResultA(o, e);
        D_1F80019E = 0;
    }
}
