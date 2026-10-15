// FUNC 8011f568 80 X004
// MATCHING 8011f568 80
#include "spad.h"
#include "TOBJ.H"

extern short boxesOverlapSide(TObj *, TObj *);
extern void resolveHitResultA(TObj *, TObj *);

void shuttlePlatformObjectCollision(TObj *o, TObj *e)
{
    if (boxesOverlapSide(o, e) >= 0) {
        resolveHitResultA(o, e);
        SPAD.s19e = 0;
    }
}
