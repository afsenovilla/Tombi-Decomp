// FUNC 801278c0 1820 X001
/* score 86 (opusN, was 345): rebuilt from the x001/wip/func_801244A0.c top half (short dx/dy/cx/cy, ax/ay box temps,
   sabs() inline, q/r with the dead `ay = q;` at the end for the r copy). Reassociation fixed with negated forms:
   `-(s2 + 4) + e->y`, `-(s2 + -4) + e->y`, `e->h - -(s7 + 0xc)`, `e->h - -(s7 + -0xc)`; compares written with the
   (ky/kx) side first (`-(s2+4) + e->y > o->y + box3 - box2`, `e->h - -(s7 + -0xc) > o->h + (box1 - box0)`); last
   d8c written `if (ang >= 0x800) (ang+0x800)... else ang >> 4`. sabs() without the $3 clobber here (game v1).
   Left: box0 temp a1/a3 swap at the top; s-reg permutation (game cx s3, dy s4, kx s5, dx s6; ours dy s3, kx s4,
   dx s5, cx s6); abs(cy) test reads ay (game tests cy: `if (cy < 0) ay = -cy` still cse'd; int cx/cy 149). */
/* Older: score 345 (was 417; size now 1820 = game). slope/box push-out (o vs e, mode, angle). Found: ax/ay short (compared as
   short, == 0 tests), v unsigned with `(..)+0xc00 / else` form (srl, andi per arm), the b9c / type-step-state exits are
   plain `return;` (v0 = the loaded field in the game), quadrant base copied into a second var b, first x-block written
   `ang >= 0x800 ? .. : ang >> 4` so ang>>4 is the shared tail. Left: gcc here reassociates e->y - (ky + 4) into
   (e->y - 4) - ky and e->h + (kx + 12) into (e->h + 12) + kx (the game keeps ky+4 / kx+12 first); the same for the last
   x-block pair; register choice (box1 $8, dx sum $5, abs temps); last-block d8c tail order. -fno-expensive-optimizations
   gives 340 but moves e to $19. */
#include "TOBJ.H"

extern int rcos(int);
extern int rsin(int);

static __inline__ short sabs(short x)
{
    short r;
    if (x < 0) r = -x; else r = x;
    return r;
}

int func_801278C0(TObj *o, TObj *e, unsigned char mode, int ang)
{
    short dx, dy, cx, cy, ax, ay, s2, s7;
    short kind;
    unsigned int v;
    int q, r;

    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) >= 0x5b) return 0;
    dx = o->h->p.whole - e->h->p.whole;
    ax = e->box0 + dx;
    if ((unsigned short)ax > e->box1) return 0;
    dy = o->y.p.whole - e->y.p.whole;
    ay = e->box0 + dy;
    if ((unsigned short)ay > e->box1) return 0;
    switch (mode) {
    case 1:
    case 2:
        q = (dy < 0) ? 0xc00 : 0;
        r = q;
        if (dx > 0) {
            if (q == 0) r = 0x400;
            else r = q - 0x400;
        }
        if ((unsigned)(((ang - r) & 0xfff) - 0x500) > 0x900) return 1;
    case 0:
        cx = (unsigned)(rcos(ang) * e->box0) >> 12;
        cy = (unsigned)(rsin(ang) * e->box0) >> 12;
    }
    ax = cx;
    ay = cy;
    if (cx < 0) ax = -cx;
    if (ay < 0) ay = -ay;
    kind = ax < ay;
    if (ax == 0) kind = 3;
    else if (ay == 0) kind = 2;
    switch (kind) {
    case 0:
        if (ax < sabs(dx)) return 2;
        s2 = dx * cy / cx;
        break;
    case 1:
        if (ay < sabs(dy)) return 2;
        s7 = dy * cx / -cy;
        break;
    case 2:
        s2 = 0;
        break;
    case 3:
        s7 = 0;
        break;
    }
    if (!(kind & 1)) {
        if (o->y.p.whole < e->y.p.whole - s2) {
            if (-(s2 + 4) + e->y.p.whole > o->y.p.whole + o->box3 - o->box2) return 3;
            o->y.p.whole = (-(s2 + 4) + e->y.p.whole) - (o->box3 - o->box2);
            o->b69 = 1;
            if (o->category != 2) return 2;
            if (o->b9c) return;
            if (o->type == 0 && o->step == 2) return;
            if ((unsigned)(ang - 0x401) < 0x3ff || ang >= 0xc01) v = (ang & 0x3ff) + 0xc00;
            else v = ang & 0x3ff;
            o->d8c = v >> 4;
            return;
        } else {
            if (-(s2 + -4) + e->y.p.whole < o->y.p.whole - o->box2) return 3;
            o->y.p.whole = o->box2 + (-(s2 + -4) + e->y.p.whole);
            if (o->category != 2) return 2;
            o->b69 = 4;
            if (o->b9c == 1) {
                if (o->velV < 0) o->velV = 0;
                return;
            }
            if (o->b9c) return;
            if (o->type == 0 && o->step == 2) return;
            if ((unsigned)(ang - 0x401) < 0x3ff || ang >= 0xc01) v = (ang & 0x3ff) + 0xc00;
            else v = ang & 0x3ff;
            o->d8c = ((v >> 4) + 0x80) & 0xff;
            return;
        }
    }
    if (o->category == 2 && o->type != 0) {
        if (e->h->p.whole + s7 < o->h->p.whole) {
            if (e->h->p.whole - -(s7 + 0xc) < o->h->p.whole) return 3;
            o->h->p.whole = e->h->p.whole - -(s7 + 0xc);
            o->b9d = 3;
            if (ang < 0x800) o->d8c = ((ang + 0x800) & 0xfff) >> 4;
            else o->d8c = ang >> 4;
        } else {
            if (o->h->p.whole < e->h->p.whole - -(s7 + -0xc)) return 3;
            o->h->p.whole = e->h->p.whole - -(s7 + -0xc);
            o->b9d = 2;
            if (ang >= 0x800) o->d8c = ((ang + 0x800) & 0xfff) >> 4;
            else o->d8c = ang >> 4;
        }
        return 3;
    }
    if (e->h->p.whole + s7 < o->h->p.whole) {
        if (e->h->p.whole - -(s7 + 0xc) < o->h->p.whole - o->box0) return 3;
        o->h->p.whole = o->box0 + (e->h->p.whole - -(s7 + 0xc));
        o->b9d = 3;
        if (o->category != 2) return 2;
        if (o->b9c) return;
        if (o->type == 0 && o->step == 2 && o->state != 2) return;
        if (ang < 0x800) o->d8c = ((ang + 0x800) & 0xfff) >> 4;
        else o->d8c = ang >> 4;
    } else {
        if (e->h->p.whole - -(s7 + -0xc) > o->h->p.whole + (o->box1 - o->box0)) return 3;
        o->h->p.whole = (e->h->p.whole - -(s7 + -0xc)) - (o->box1 - o->box0);
        o->b9d = 2;
        if (o->category != 2) return 2;
        if (o->b9c) return;
        if (o->type == 0 && o->step == 2 && o->state != 2) return;
        if (ang >= 0x800) o->d8c = ((ang + 0x800) & 0xfff) >> 4;
        else o->d8c = ang >> 4;
    }
    ay = q;
}
