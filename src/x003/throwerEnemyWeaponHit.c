// FUNC 8011f7ec 224 X003
// MATCHING 8011f7ec 224
#include "spad.h"
#include "TOBJ.H"
extern short boxesOverlapSigned(TObj *, TObj *);
extern int func_80042B98(TObj *, TObj *);
extern void awardAPByArea(int, int, int, int);

void throwerEnemyWeaponHit(TObj *p, TObj *o)
{
    int r;

    if (boxesOverlapSigned(p, o) == -1)
        return;
    o->w7a = p->animFrame & 1;
    r = func_80042B98(p, o);
    if (r != 0) {
        if (r == 1) {
            if (o->subtype != 0)
                o->active = 3;
            else
                o->active = 2;
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
        } else {
            awardAPByArea(0x1f4, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            o->active = 2;
            o->animFrame = 1 - (p->animFrame & 1);
            o->b04 = 2;
            o->step = 2;
            o->state = 0;
        }
    }
    SPAD.s19e = 0;
}
