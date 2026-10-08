// FUNC 8011f5b4 72 X010
// MATCHING 8011f5b4 72
#include "TOBJ.H"
extern short func_80044550(TObj *, TObj *);
extern void func_800428C0(TObj *, TObj *);

void func_8011F5B4(TObj *a, TObj *b)
{
    if (func_80044550(a, b) >= 0) func_800428C0(a, b);
}
