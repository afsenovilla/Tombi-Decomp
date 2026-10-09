/* score 4: (o38: brute-forced ~250 expression forms: box3 temp, b->box2 temp, casts, inline sub/neg; all >=4) y check: game loads a->box3 before a->box2 and adds w + b->box2 (addu v1,a0,v1); this form (-a->box2 + a->box3, rhs a->box3 + b->box3 + 0x40) gets the registers right but loads box2 first. With a->box3 - a->box2 the load order is right but a->box3/b->box2 swap v0/v1 (score 24). Tried operand orders, temps, inline add/sub, local type brute force. */
// FUNC 80127700 396 X001
#include "TOBJ.H"

int func_80127700(TObj *a, TObj *b)
{
    char pad;
    short dx, dy, w, k;

    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return 0;
    if ((unsigned short)(b->box0 + (a->h->p.whole - b->h->p.whole)) > b->box1) return 0;
    if ((unsigned short)(a->y.p.whole - b->y.p.whole + ((w = -a->box2 + a->box3) + b->box2)) > a->box3 + b->box3 + 0x40) return 0;
    dx = b->d30 - b->h->p.whole;
    dy = b->d34 - b->y.p.whole;
    k = a->h->p.whole - (b->h->p.whole - b->box0);
    if (k <= 0) k = 0;
    else k = k * dy / dx;
    if (b->y.p.whole + k > a->y.p.whole + w) return 0;
    a->y.p.whole = b->y.p.whole + k - w;
    a->y.p.frac = 0;
    a->b69 = 1;
    a->d38 = b->d8c;
    return 1;
}
