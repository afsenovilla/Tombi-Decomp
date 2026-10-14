// FUNC 8011ddc0 152 X010
// MATCHING 8011ddc0 152
#include "spad.h"
#include "TOBJ.H"

extern short FUN_8004461c(TObj *a, TObj *b);
extern int resolveHitResultD(TObj *, TObj *);
extern void FUN_800e9f74(int, int, int, int);

void func_8011DDC0(TObj *o, TObj *p)
{
    if (FUN_8004461c(o, p) == -1) return;
    if (resolveHitResultD(o, p)) {
        FUN_800e9f74(0x1f4, p->a.p.whole, p->y.p.whole, p->b.p.whole);
        p->active = 2;
        p->animFrame = ~o->animFrame & 1;
        p->b04 = 2;
        p->step = 2;
        p->state = 0;
    }
    SPAD.s19e = 0;
}
