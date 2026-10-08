// FUNC 8011a1e0 68 X010
// MATCHING 8011a1e0 68
#include "TOBJ.H"

void func_8011A1E0(TObj *o)
{
    o->active = 2;
    o->visible = ((TObj *)o->d90)->visible;
    o->a.raw = ((TObj *)o->d90)->a.raw + o->d30;
    o->y.raw = ((TObj *)o->d90)->y.raw + o->d34;
}
