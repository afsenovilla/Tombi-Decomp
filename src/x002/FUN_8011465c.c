// FUNC 8011465c 48 X002
// MATCHING 8011465c 48
#include "TOBJ.H"
extern void l2Type18Hit(TObj *o);
void FUN_8011465c(TObj *o)
{
    if (o->step == 0) l2Type18Hit(o);
}
