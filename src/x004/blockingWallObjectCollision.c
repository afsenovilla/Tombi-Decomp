// FUNC 8011f41c 192 X004
// MATCHING 8011f41c 192
#include "spad.h"
#include "TOBJ.H"
extern short boxesOverlapSide(TObj *, TObj *);
extern int resolveHitResultB(TObj *, TObj *);
extern void playSFX(int);
extern void FUN_8001f96c(int, short, short, short);

void blockingWallObjectCollision(TObj *o, TObj *e)
{
    int r;
    if (boxesOverlapSide(o, e) >= 0) {
        r = resolveHitResultB(o, e);
        if (r) {
            if (r == 7 && o->subtype == 0) {
                e->active = 2;
                e->b04 = 2;
                e->step = 0;
                e->state = 0;
                playSFX(7);
                FUN_8001f96c(0, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            } else {
                playSFX(5);
                FUN_8001f96c(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            }
        }
        SPAD.s19e = 0;
    }
}
