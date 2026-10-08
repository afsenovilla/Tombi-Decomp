// FUNC 80126d7c 492 X000
/* full-address score 44 (b29; was 114): dy<=0 branch recomputes e->y - (e->box2 + (o->box3 - o->box2)) (fixed the o/e a2/a3 swap), t = o->box3 first.
   Left: dy copy (move t2,a1): game has no copy because the subtraction is combined into dy (temp used once); ours CSE
   replaces dy by the subtraction temp in the sum, and dy survives to the dy<=0 test at a join point (after the dx<0 block),
   unlike matched FUN_8004886c where CSE kills dy entirely. Shifts t2..t4. Tried dy int/ushort, (short) casts, 2-step dy,
   dy after the test, ~10 sum forms, -fno-cse-follow-jumps/skip-blocks, type brute force. Old note: score 112 (was 118): rewritten in FUN_8004886c style (t = e->box2 + (o->box3 - o->box2)). Left: o/e register swap (game e=a2 copied first, o=a3 in the beq delay slot; ours o=a2, e=a3) and the dy/t register choice in the y test. Tried: plain/local-copy params in both orders, rest-of-body as static inline (void/int), alias pointer per o->/e-> access (greedy; one oo-> store gives 90 but with garbage copies). */
#include "TOBJ.H"
#define B(o, n) (((unsigned char *)(o))[n])
void FUN_8004886c(TObj *o, TObj *e);

void func_80126D7C(TObj *o, TObj *e)
{
    short sx, dx, dy, w, a, t;
    if (e->subtype != 2) {
        FUN_8004886c(o, e);
        return;
    }
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) >= 0x5b)
        return;
    if (o->animFrame & 1)
        sx = o->box0;
    else
        sx = o->box1 - o->box0;
    dx = o->h->p.whole - e->h->p.whole;
    w = e->box0 + sx;
    if ((unsigned short)dx > e->box1 + o->box1)
        return;
    dy = o->y.p.whole - e->y.p.whole;
    t = o->box3; t = e->box2 + (t - o->box2);
    if ((unsigned short)(dy + t) > o->box3 + e->box3)
        return;
    if (dx < 0) {
        a = -dx;
        if ((unsigned short)(w - a) < 4) {
            dx = -w;
            if (w == a)
                return;
            o->h->p.whole = e->h->p.whole + dx;
            B(o, 0x9d) = 2;
            return;
        }
    }
    if (dy <= 0) {
        if (o->b9c & 1)
            return;
        o->y.p.whole = e->y.p.whole - (e->box2 + (o->box3 - o->box2));
        o->y.p.frac = 0;
        o->b69 = 1;
        return;
    }
    if (o->category == 2 && *(unsigned short *)&o->b04 == 0x102)
        return;
    o->y.p.whole = e->y.p.whole + (o->box2 + (e->box3 - e->box2));
}
