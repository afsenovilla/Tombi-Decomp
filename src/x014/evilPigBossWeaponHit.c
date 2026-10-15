// FUNC 8011d2dc 148 X014
// MATCHING 8011d2dc 148
#include "spad.h"
#include "TOBJ.H"
extern short boxesOverlapFlipAware(TObj *, TObj *);
extern void resolveHitResultA(TObj *, TObj *);
extern void spawnImpactEffect(int, int, int, int);

void evilPigBossWeaponHit(TObj *o, TObj *p)
{
    if (boxesOverlapFlipAware(o, p) == -1) return;
    resolveHitResultA(o, p);
    if (o->type == 10) spawnImpactEffect(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
    p->active = 2;
    p->b04 = 1;
    p->step = 0;
    p->state = 0;
    SPAD.s19e = 0;
}
