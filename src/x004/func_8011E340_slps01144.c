// VERSION SLPS_01144 x04 8011E340 320
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"

extern short boxesOverlapSigned(TObj *, TObj *);
extern short resolveHitResultD(TObj *, TObj *);
extern void func_800EB894(int, int, int, int);

void func_8011E340_slps01144(TObj *a, TObj *o)
{
    short r;
    int one;
    unsigned short t, t1, t2;

    if (boxesOverlapSigned(a, o) == -1) return;
    r = resolveHitResultD(a, o);
    if (r) {
        asm("" : "=r"(one) : "0"(1));
        if (r == one) {
            o->active = 3;
            t1 = a->animFrame;
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
            o->animFrame = t1 & 1;
            if (a->type == 6) {
                o->w98 -= 2;
                if (o->w98 <= 0) {
                    o->w98 = 0;
                    func_800EB894(500, o->a.p.whole, o->y.p.whole, o->b.p.whole);
                    o->active = 2;
                    t2 = a->animFrame;
                    o->b04 = 2;
                    o->step = 2;
                    o->state = 0;
                    o->animFrame = one - (t2 & 1);
                }
            }
        } else {
            func_800EB894(500, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            o->active = 2;
            t = a->animFrame;
            o->b04 = 2;
            o->step = 2;
            o->state = 0;
            o->animFrame = one - (t & 1);
        }
    }
    SPAD.s19e = 0;
}
#endif
