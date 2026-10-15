// FUNC 80126004 328 X001
// MATCHING 80126004 328
#include "gcd.h"
#include "TOBJ.H"
extern short boxesOverlapSide(TObj *, TObj *);
extern void resolveHitResultA(TObj *, TObj *);
extern int resolveHitResultB(TObj *, TObj *);
extern void playSFX(int);
extern void spawnImpactEffect(int, int, int, int);
extern void func_80132698(int, int, int);
extern void FUN_8002ee50(int, int, int, int);

void func_80126004(TObj *o, TObj *e)
{
    int r;

    if (boxesOverlapSide(o, e) < 0) {
        return;
    }
    if (GCD.bceb0) {
        resolveHitResultA(o, e);
        return;
    }
    r = resolveHitResultB(o, e);
    if (r == 0) {
        return;
    }
    if (r >= 3) {
        if (r != 6) {
            playSFX(5);
        }
        spawnImpactEffect(0, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        e->active = 2;
        e->b04 = 2;
        e->step = 0;
        func_80132698(e->h->p.whole, e->y.p.whole, e->d->p.whole);
        FUN_8002ee50(0, e->h->p.whole, e->y.p.whole, e->d->p.whole);
        return;
    }
    if (r == 2) {
        e->w98 = 0;
        playSFX(5);
        spawnImpactEffect(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
    } else {
        if (--e->w98 <= 0) {
            e->w98 = 0;
        }
        playSFX(5);
        spawnImpactEffect(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
    }
}
