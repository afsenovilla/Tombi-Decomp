// FUNC 8011d370 136 X014
// MATCHING 8011d370 136
#include "TOBJ.H"
extern short func_80043260(TObj *, TObj *);

void func_8011D370(TObj *o, TObj *p)
{
    short r = func_80043260(o, p);
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
