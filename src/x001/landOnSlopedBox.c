// FUNC 80127700 396 X001
// MATCHING 80127700 396
// FLAGS -O2 -G0 -fno-force-mem
/* Matched (opusAB): -fno-force-mem keeps (w=...) + b->box2 as (plus w box2) (expand_binop swaps a SUBREG op0 with a
   forced-to-REG mem op1 otherwise); d30/d34 read into int temps (lw) and b->y.p.whole as a short temp `by`. */
#include "TOBJ.H"

int landOnSlopedBox(TObj *a, TObj *b)
{
    char pad;
    short dx, dy, w, k;
    unsigned short ty;
    short a3;

    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return 0;
    if ((unsigned short)(b->box0 + (a->h->p.whole - b->h->p.whole)) > b->box1) return 0;
    a3 = a->box3;
    ty = (unsigned short)(a->y.p.whole - b->y.p.whole + ((w = -a->box2 + a->box3) + b->box2));
    if (ty > a3 + b->box3 + 0x40) return 0;
    { int x = b->d30, y = b->d34; dx = x - b->h->p.whole;
    dy = y - b->y.p.whole; }
    k = a->h->p.whole - (b->h->p.whole - b->box0);
    if (k <= 0) k = 0;
    else k = k * dy / dx;
    { short by = b->y.p.whole; if (by + k > a->y.p.whole + w) return 0;
    a->y.p.whole = by + k - w; }
    a->y.p.frac = 0;
    a->b69 = 1;
    a->d38 = b->d8c;
    return 1;
}
