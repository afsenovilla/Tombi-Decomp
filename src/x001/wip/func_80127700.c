/* score 18: only v0/v1 swapped in the third (y) check: game loads a->box3 into v0 and b->box2 into v1 (local-alloc priority); tried operand orders, short/int temps for a->box3, w assigned separately, inline wrapper, local types brute force. */
// FUNC 80127700 396 X001
#include "TOBJ.H"

int func_80127700(TObj *a, TObj *b)
{
    char pad;
    short dx, dy, w, k;

    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return 0;
    if ((unsigned short)(b->box0 + (a->h->p.whole - b->h->p.whole)) > b->box1) return 0;
    if ((unsigned short)(a->y.p.whole - b->y.p.whole + ((w = a->box3 - a->box2) + b->box2)) > b->box3 + 0x40 + a->box3) return 0;
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
