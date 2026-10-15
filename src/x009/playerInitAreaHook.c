// FUNC 8011d334 68 X009
// MATCHING 8011d334 68
#include "gcd.h"
#include "TOBJ.H"

extern void FUN_80030b9c(int, int, int, int);

void playerInitAreaHook(TObj *o)
{
    GCD.bcef7 = 0;
    if (GCD.bcff8 != 0) {
        FUN_80030b9c(0, o->a.p.whole, o->y.p.whole, o->b.p.whole);
    }
}
