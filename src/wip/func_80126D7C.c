// FUNC 80126d7c 492 X000
/* score 112 (was 118): rewritten in FUN_8004886c style (t = e->box2 + (o->box3 - o->box2)). Left: o/e register swap (game e=a2 copied first, o=a3 in the beq delay slot; ours o=a2, e=a3) and the dy/t register choice in the y test. Tried: plain/local-copy params in both orders, rest-of-body as static inline (void/int), alias pointer per o->/e-> access (greedy; one oo-> store gives 90 but with garbage copies). */
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
    t = e->box2 + (o->box3 - o->box2);
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
        o->y.p.whole = e->y.p.whole - t;
        o->y.p.frac = 0;
        o->b69 = 1;
        return;
    }
    if (o->category == 2 && *(unsigned short *)&o->b04 == 0x102)
        return;
    o->y.p.whole = e->y.p.whole + (o->box2 + (e->box3 - e->box2));
}
