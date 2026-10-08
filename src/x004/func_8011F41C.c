// FUNC 8011f41c 192 X004
// MATCHING 8011f41c 192
#include "TOBJ.H"
extern short D_1F80019E;
extern short func_80044550(TObj *, TObj *);
extern int func_800429D0(TObj *, TObj *);
extern void playSFX(int);
extern void FUN_8001f96c(int, short, short, short);

void func_8011F41C(TObj *o, TObj *e)
{
    int r;
    if (func_80044550(o, e) >= 0) {
        r = func_800429D0(o, e);
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
        D_1F80019E = 0;
    }
}
