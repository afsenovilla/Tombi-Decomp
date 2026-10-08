// FUNC 8011f844 308 X004
/* score 19: the game keeps a constant 1 in s2 across both calls and uses it for the r == 1 test and every
   `1 - (animFrame & 1)`; with a short `one` the compare becomes li 1, with int `one` the else path folds to nor/andi.
   The type==6 path duplicates the spawn call's argument setup and cross-jumps into the jal. */
#include "TOBJ.H"
extern short func_8004461C(TObj *, TObj *);
extern short FUN_80042b98(TObj *, TObj *);
extern void FUN_8002b920(TObj *);
extern void FUN_800e9f74(int, int, int, int);
extern short D_1F80019E;

void func_8011F844(TObj *a, TObj *o)
{
    short r;
    short one;
    unsigned short t;

    if (func_8004461C(a, o) == -1) return;
    r = FUN_80042b98(a, o);
    if (r != 0) {
        one = 1;
        if (r == one) {
            if (a->type == 6 && (o->w98 -= 2) <= 0) {
                o->w98 = 0;
                FUN_800e9f74(500, o->a.p.whole, o->y.p.whole, o->b.p.whole);
                o->active = 2;
                t = a->animFrame;
                o->b04 = 2;
                o->step = 2;
                o->state = 0;
                o->animFrame = one - (t & 1);
                goto end;
            }
            o->active = 3;
            o->animFrame = one - (a->animFrame & 1);
            if (o->b04 != 2)
                FUN_8002b920(o);
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
        } else {
            FUN_800e9f74(500, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            o->active = 2;
            t = a->animFrame;
            o->b04 = 2;
            o->step = 2;
            o->state = 0;
            o->animFrame = one - (t & 1);
        }
    }
end:
    D_1F80019E = 0;
}
