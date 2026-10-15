// FUNC 8011fec8 156 X003
// MATCHING 8011fec8 156
#include "spad.h"
#include "TOBJ.H"
extern short boxesOverlapSigned(TObj *, TObj *);
extern int resolveHitResultA(TObj *, TObj *);

void areaInitAObjectCollision(TObj *o, TObj *e)
{
    if (e->d94 == 0 && boxesOverlapSigned(o, e) >= 0) {
        resolveHitResultA(o, e);
        if (o->type != 1) {
            if (o->animFrame < 4) e->animFrame = o->animFrame & 1;
            else e->animFrame = 3;
        } else {
            e->animFrame = o->animFrame & 1;
        }
        SPAD.s19e = 0;
    }
}
