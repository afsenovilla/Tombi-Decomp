// FUNC 801178b0 124 X009
// MATCHING 801178b0 124
#include "TOBJ.H"

extern void func_8011792C(TObj *);
extern void func_80117A9C(TObj *);
extern void func_80117BF0(TObj *);

void func_801178B0(TObj *o)
{
    switch (o->step) {
    case 0:
        func_8011792C(o);
        break;
    case 1:
        func_80117A9C(o);
        break;
    case 2:
        func_80117BF0(o);
        break;
    }
}
