// FUNC 8011f4dc 140 X004
// MATCHING 8011f4dc 140
#include "TOBJ.H"
extern short D_1F80019E;
extern short func_80043260(TObj *, TObj *);

void func_8011F4DC(TObj *o, TObj *e)
{
    if (o->b9e == 0) {
        e->b69 = 0;
        if (func_80043260(o, e) == 1) {
            D_1F80019E = 0;
            o->h->raw += e->velH << 8;
            o->y.raw += e->velV << 8;
        }
    }
}
