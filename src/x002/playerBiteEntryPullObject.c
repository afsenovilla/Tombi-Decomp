// FUNC 80104040 144 X002
// MATCHING 80104040 144
#include "TOBJ.H"
extern TObj *BITTEN_OBJECT;
extern void PlayerSetAnimIfChanged(TObj *, int);
extern void spawnImpactEffect(int, int, int, int);

void playerBiteEntryPullObject(TObj *o)
{
    TObj *p;
    o->timer = 0;
    PlayerSetAnimIfChanged(o, 0xd);
    spawnImpactEffect(2, o->a.p.whole, o->y.p.whole, o->b.p.whole);
    *(char *)((char *)o + 0xac) = 3;
    o->b9c = 0;
    p = BITTEN_OBJECT;
    o->velX = 0;
    o->velY = 0;
    p->h->p.whole = o->h->p.whole;
    *(short *)((char *)p + 0x16) = o->y.p.whole + *(short *)((char *)p + 0x70);
    o->state = 2;
}
