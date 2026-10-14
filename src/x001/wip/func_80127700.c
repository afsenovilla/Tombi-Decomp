/* opusP: still 2. RTL dump (-dj/-dg) shows expand already emits (plus box2 w-value) for every source order, so source operand swaps cannot reach it; tried int/short temps for b->box2, w as separate stmt (102), (short) casts, *&b->box2, |0, reassociated sums (10-111), int/ushort w (68/17), asm bump on w. */
/* o41: still 2. Tried: b->box2 + (w=..) operand swaps, (unsigned short)b->box2, w/t2 temps, int w2 temp, `// CC gcc-2.8.1` (93), `// AS gnu` (19), asm("":::"$2".."$5") at several points (2 or worse), asm volatile("") (39); permuter rerun ~40k more iterations (pipeline cc1+maspsx+as) found nothing below base. */
/* permuter (n80): `a3 = a->box3; ty = (ushort)(...); if (ty > a3 + b->box3 + 0x40)` fixes the box3/box2 load order (decomp-permuter found it; 100k+ more iterations from here found nothing better). Only left: addu v1,a0,v1 (game) vs addu v1,v1,a0 (operands swapped, ~3 other forms keep it). */
/* score 4: (o38: brute-forced ~250 expression forms: box3 temp, b->box2 temp, casts, inline sub/neg; all >=4) y check: game loads a->box3 before a->box2 and adds w + b->box2 (addu v1,a0,v1); this form (-a->box2 + a->box3, rhs a->box3 + b->box3 + 0x40) gets the registers right but loads box2 first. With a->box3 - a->box2 the load order is right but a->box3/b->box2 swap v0/v1 (score 24). Tried operand orders, temps, inline add/sub, local type brute force. */
// FUNC 80127700 396 X001
#include "TOBJ.H"

int func_80127700(TObj *a, TObj *b)
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
