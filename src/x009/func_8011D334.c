// FUNC 8011d334 68 X009
// MATCHING 8011d334 68
#include "TOBJ.H"

extern unsigned char D_8009CFF8;
extern unsigned char D_8009CEF7;
extern void FUN_80030b9c(int, int, int, int);

void func_8011D334(TObj *o)
{
    D_8009CEF7 = 0;
    if (D_8009CFF8 != 0) {
        FUN_80030b9c(0, o->a.p.whole, o->y.p.whole, o->b.p.whole);
    }
}
