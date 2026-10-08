// FUNC 8011f418 92 X003
// MATCHING 8011f418 92
#include "TOBJ.H"
extern short func_8004461C(TObj *, TObj *);
extern int func_800428C0(TObj *, TObj *);
extern short D_1F80019E;

void func_8011F418(TObj *o, TObj *p)
{
    if (func_8004461C(o, p) >= 0) {
        func_800428C0(o, p);
        D_1F80019E = 0;
        p->animFrame = o->animFrame & 1;
    }
}
