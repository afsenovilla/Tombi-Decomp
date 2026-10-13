// FUNC 8011df3c 80 X010
// MATCHING 8011df3c 80
#include "TOBJ.H"

extern short D_1F80019E;
extern short boxesOverlapSigned(TObj *, TObj *);
extern void resolveHitResultA(TObj *, TObj *);

void func_8011DF3C(TObj *o, TObj *e)
{
    if (boxesOverlapSigned(o, e) >= 0) {
        resolveHitResultA(o, e);
        D_1F80019E = 0;
    }
}
