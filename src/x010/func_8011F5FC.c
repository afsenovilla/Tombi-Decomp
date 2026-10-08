// FUNC 8011f5fc 72 X010
// MATCHING 8011f5fc 72
#include "TOBJ.H"

void func_8011F5FC(TObj *o)
{
    if (o->step == 4 || o->step == 0x41) {
        o->step = 0x41;
    } else if (o->step == 10) {
        o->step = 4;
        o->state = 3;
    } else {
        o->step = 0x3f;
    }
}
