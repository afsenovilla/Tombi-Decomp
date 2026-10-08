// FUNC 8011fa74 116 X004
// MATCHING 8011fa74 116
#include "TOBJ.H"
extern short D_1F80019E;
extern short func_800482EC(TObj *, TObj *);

void func_8011FA74(TObj *o, TObj *e)
{
    if (func_800482EC(o, e)) {
        D_1F80019E = 0;
        o->h->raw += e->velH << 8;
        o->y.raw += e->velV << 8;
    }
}
