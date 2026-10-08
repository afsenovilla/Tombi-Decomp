// FUNC 8011d2dc 148 X014
// MATCHING 8011d2dc 148
#include "TOBJ.H"
extern short func_800446DC(TObj *, TObj *);
extern void func_800428C0(TObj *, TObj *);
extern void FUN_8001f96c(int, int, int, int);
extern short D_1F80019E;

void func_8011D2DC(TObj *o, TObj *p)
{
    if (func_800446DC(o, p) == -1) return;
    func_800428C0(o, p);
    if (o->type == 10) FUN_8001f96c(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
    p->active = 2;
    p->b04 = 1;
    p->step = 0;
    p->state = 0;
    D_1F80019E = 0;
}
