// FUNC 8011df3c 80 X010
// MATCHING 8011df3c 80
#include "TOBJ.H"

extern short D_1F80019E;
extern short func_8004461C(TObj *, TObj *);
extern void func_800428C0(TObj *, TObj *);

void func_8011DF3C(TObj *o, TObj *e)
{
    if (func_8004461C(o, e) >= 0) {
        func_800428C0(o, e);
        D_1F80019E = 0;
    }
}
