// FUNC 8011f568 80 X004
// MATCHING 8011f568 80
#include "TOBJ.H"

extern short D_1F80019E;
extern short func_80044550(TObj *, TObj *);
extern void func_800428C0(TObj *, TObj *);

void func_8011F568(TObj *o, TObj *e)
{
    if (func_80044550(o, e) >= 0) {
        func_800428C0(o, e);
        D_1F80019E = 0;
    }
}
