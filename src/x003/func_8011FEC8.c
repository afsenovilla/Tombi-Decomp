// FUNC 8011fec8 156 X003
// MATCHING 8011fec8 156
#include "TOBJ.H"
extern short D_1F80019E;
extern short boxesOverlapSigned(TObj *, TObj *);
extern int resolveHitResultA(TObj *, TObj *);

void func_8011FEC8(TObj *o, TObj *e)
{
    if (e->d94 == 0 && boxesOverlapSigned(o, e) >= 0) {
        resolveHitResultA(o, e);
        if (o->type != 1) {
            if (o->animFrame < 4) e->animFrame = o->animFrame & 1;
            else e->animFrame = 3;
        } else {
            e->animFrame = o->animFrame & 1;
        }
        D_1F80019E = 0;
    }
}
