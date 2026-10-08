// FUNC 8011f978 104 X004
// MATCHING 8011f978 104
#include "TOBJ.H"
extern short func_80044550(TObj *, TObj *);
extern int func_800428C0(TObj *, TObj *);
extern short D_1F80019E;

void func_8011F978(TObj *o, TObj *p)
{
    if (p->b0c == 0 && func_80044550(o, p) != -1) {
        func_800428C0(o, p);
        D_1F80019E = 0;
    }
}
