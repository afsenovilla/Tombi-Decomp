// FUNC 80124a84 312 X000
/* score 2: only `andi a0,0xffff` (zext of the box2+dy sum) is scheduled before `subu v1,v1,a1` (e) instead of after.
   b17: 12->2 by u5 unsigned short / t short (game recomputes u6-u5 for t) with `w = u6; w -= u5;` for the first test
   (reassigning w kills the CSE of the minus) and ah/bh pointer locals loaded first. Tried for the rest: e before the if,
   ushort/short/int temps for the sum or dy (one or two steps), & 0xffff, reversed compare, casts on each operand. */
#include "TOBJ.H"

int FUN_80124a84(TObj *a, TObj *b)
{
    unsigned short u5; int u6; int w;
    short d; short e; short s; short t;
    Fix16 *ah = a->h, *bh = b->h;
    u5 = (unsigned short)b->box0;
    u6 = (unsigned short)ah->p.whole;
    w = u6; w -= u5;
    d = bh->p.whole - u5;
    if ((unsigned short)w <= d) {
        if ((unsigned short)(a->box2 + (a->y.p.whole - b->y.p.whole)) > (e = b->box2 - b->y.p.whole) + a->box3)
            return 0;
        t = u6 - u5;
        if (t <= 0)
            s = 0;
        else
            s = t * e / d;
        if (b->box2 - s <= a->y.p.whole + a->box2) {
            a->y.p.whole = b->box2 - s - a->box2;
            a->y.p.frac = 0;
            a->velY = 0;
            a->b69 = 1;
            return 1;
        }
    }
    return 0;
}
