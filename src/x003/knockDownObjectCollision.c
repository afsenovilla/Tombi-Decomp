// FUNC 8011f418 92 X003
// MATCHING 8011f418 92
#include "TOBJ.H"
extern short boxesOverlapSigned(TObj *, TObj *);
extern int resolveHitResultA(TObj *, TObj *);
extern short LIST_COUNTER;

void knockDownObjectCollision(TObj *o, TObj *p)
{
    if (boxesOverlapSigned(o, p) >= 0) {
        resolveHitResultA(o, p);
        LIST_COUNTER = 0;
        p->animFrame = o->animFrame & 1;
    }
}
