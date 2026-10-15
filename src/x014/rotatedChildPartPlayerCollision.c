// FUNC 8011d370 136 X014
// MATCHING 8011d370 136
#include "TOBJ.H"
extern short pushObjectOutOfObjectBoxA(TObj *, TObj *);

void rotatedChildPartPlayerCollision(TObj *o, TObj *p)
{
    short r = pushObjectOutOfObjectBoxA(o, p);
    if (r != 0 && r == 1) {
        if (o->h->p.whole > p->h->p.whole) {
            o->bbe = 8;
            o->wb0 = 2;
        } else {
            o->bbe = 9;
            o->wb0 = -2;
        }
    }
}
