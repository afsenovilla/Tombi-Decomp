// FUNC 8011cba0 124 X014
// MATCHING 8011cba0 124
#include "TOBJ.H"

extern short D_1F80019E;
extern short func_8004461C(TObj *, TObj *);
extern int FUN_80042b98(TObj *, TObj *);
extern void FUN_8002ee50(int, int, int, int);

void func_8011CBA0(TObj *o, TObj *e)
{
    if (func_8004461C(o, e) >= 0) {
        if (FUN_80042b98(o, e)) {
            e->active = 2;
            e->b04 = 2;
            e->step = 0;
            e->state = 0;
            FUN_8002ee50(0, e->a.p.whole, e->y.p.whole, e->b.p.whole);
        }
        D_1F80019E = 0;
    }
}
