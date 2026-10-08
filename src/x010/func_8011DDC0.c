// FUNC 8011ddc0 152 X010
// MATCHING 8011ddc0 152
#include "TOBJ.H"

extern short D_1F80019E;
extern short FUN_8004461c(TObj *a, TObj *b);
extern int FUN_80042b98(TObj *, TObj *);
extern void FUN_800e9f74(int, int, int, int);

void func_8011DDC0(TObj *o, TObj *p)
{
    if (FUN_8004461c(o, p) == -1) return;
    if (FUN_80042b98(o, p)) {
        FUN_800e9f74(0x1f4, p->a.p.whole, p->y.p.whole, p->b.p.whole);
        p->active = 2;
        p->animFrame = ~o->animFrame & 1;
        p->b04 = 2;
        p->step = 2;
        p->state = 0;
    }
    D_1F80019E = 0;
}
