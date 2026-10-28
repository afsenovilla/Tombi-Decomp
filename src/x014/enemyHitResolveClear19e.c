// FUNC 8011cff0 80 X014
// MATCHING 8011cff0 80
#include "spad.h"
#include "TOBJ.H"

extern short boxesOverlapSigned(TObj *, TObj *);
extern void resolveHitResultA(TObj *, TObj *);

void enemyHitResolveClear19e(TObj *o, TObj *e)
{
    if (boxesOverlapSigned(o, e) >= 0) {
        resolveHitResultA(o, e);
        SPAD.s19e = 0;
    }
}
