// FUNC 8011f418 92 X003
// MATCHING 8011f418 92
#include "TOBJ.H"
extern short boxesOverlapSigned(TObj *, TObj *);
extern int resolveHitResultA(TObj *, TObj *);
extern short D_1F80019E;

void knockDownObjectCollision(TObj *o, TObj *p)
{
    if (boxesOverlapSigned(o, p) >= 0) {
        resolveHitResultA(o, p);
        D_1F80019E = 0;
        p->animFrame = o->animFrame & 1;
    }
}
