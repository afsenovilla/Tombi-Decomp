// FUNC 8011faa8 104 X001
// MATCHING 8011faa8 104
#include "TOBJ.H"
extern void func_8011F2B0(TObj *);
extern void func_8011F6C4(TObj *);

void func_8011FAA8(TObj *o)
{
    if (o->visible == 0) return;
    switch (o->step) {
    case 0:
        break;
    case 1:
        func_8011F2B0(o);
        break;
    case 2:
        func_8011F6C4(o);
        break;
    }
}
