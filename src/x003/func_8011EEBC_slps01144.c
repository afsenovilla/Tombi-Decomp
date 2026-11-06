// VERSION SLPS_01144 x03 8011EEBC 400
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"

extern short boxesOverlapSigned(TObj *, TObj *);
extern int resolveHitResultD(TObj *, TObj *);
extern int resolveHitResultF(TObj *, TObj *);
extern void func_800EB894(int, int, int, int);

void func_8011EEBC_slps01144(TObj *a, TObj *o)
{
    int r;
    unsigned char v;
    
    register unsigned short t asm("v1");
    register int one asm("s2");

    if (boxesOverlapSigned(a, o) == -1) return;
    o->w7a = a->animFrame & 1;
    if (*(unsigned short *)&o->wb4) {
        if (*(unsigned short *)&o->wb6) {
            r = resolveHitResultD(a, o);
            if (r) {
                if (r == 2) v = 5;
                else if (!(a->type == 0 || a->type == 5)) v = 2;
                else if (a->w98 == 1) v = 1;
                else if (a->type || o) v = 2;  /* permuter: same value both ways, keeps the retail branch shape */
                else v = 2;
                o->b68 = v;
                o->active = 3;
                o->b04 = 2;
                o->step = 0;
                o->state = 0;
            }
        } else {
            r = resolveHitResultD(a, o);
            one = 1;
            if (r) {
                if (r == one) {
                    o->active = 3;
                    o->b04 = 2;
                    o->step = 0;
                    o->state = 0;
                } else {
                    func_800EB894(500, o->a.p.whole, o->y.p.whole, o->b.p.whole);
                    o->active = 2;
                    t = a->animFrame;
                    o->b04 = 2;
                    o->step = 2;
                    o->state = 0;
                    t = one - (t & 1);
                    o->animFrame = t;
                }
            }
        }
    } else {
        if (resolveHitResultF(a, o) && o->w98 == 0 && o->active != 5) {
            o->active = 3;
            o->b68 = 0;
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
        }
    }
    SPAD.s19e = 0;
}
#endif
