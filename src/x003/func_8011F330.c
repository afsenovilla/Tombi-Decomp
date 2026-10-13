// FUNC 8011f330 92 X003
// MATCHING 8011f330 92
#include "TOBJ.H"
extern short boxesOverlapSigned(TObj *, TObj *);
extern int resolveHitResultA(TObj *, TObj *);
extern short D_1F80019E;

void func_8011F330(TObj *o, TObj *p)
{
    if (boxesOverlapSigned(o, p) >= 0) {
        resolveHitResultA(o, p);
        D_1F80019E = 0;
        p->animFrame = o->animFrame & 1;
    }
}
