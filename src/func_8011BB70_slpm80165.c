// VERSION SLPM_80165 x00 8011BB70 200
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo-only weapon hit on a three-part chained object: on a hit (func_8004AB10 >= 0) all three parts get +0x68 =
// result | 2, an impact effect at the weapon, and sound 7 unless the weapon is of type 1.
#include "TOBJ.H"
extern short func_8004AB10(TObj *, TObj *);
extern void func_80020054(int, int, int, int);
extern void func_8001E3D8(int);

void func_8011BB70_slpm80165(TObj *w, TObj *o)
{
    TObj *p, *q;
    short r = func_8004AB10(w, o);
    if (r < 0)
        return;
    switch (o->subtype) {
    case 0:
        p = (TObj *)o->d94;
        q = (TObj *)p->d94;
        break;
    case 1:
        p = (TObj *)o->d90;
        q = (TObj *)o->d94;
        break;
    }
    o->b68 = r | 2;
    p->b68 = r | 2;
    q->b68 = r | 2;
    func_80020054(1, w->a.p.whole, w->y.p.whole, w->b.p.whole);
    if (w->type != 1)
        func_8001E3D8(7);
}
#endif
