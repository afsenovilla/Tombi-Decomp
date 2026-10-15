// FUNC 8011465c 48 X008
// MATCHING 8011465c 48
#include "TOBJ.H"
extern void l2Type18Hit(TObj *o);
void l2Type18HitIfStep0(TObj *o)
{
    if (o->step == 0) l2Type18Hit(o);
}
