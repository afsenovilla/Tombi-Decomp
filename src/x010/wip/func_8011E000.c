/* score 54 (n77): `{ int by = b->y.p.whole; ... }` temp for the D_1F8001D2-path compare fixes +0x578 (b->y a0, a->y v1).
   Left: q/r copy a3, sabs v0/v1, d v1 (ours a2) and t/w regs in the D_1F8001D2 block (game t a2 untied from the lhu,
   w a1 untied from andi); brute-forced d/t/w types x block/function scope: function-scope t/w wrecks global alloc (126+).
   Older: score 70 (n73): block-local `short t; unsigned short w;` per arm (x001 form) fixes the size but flips a/b (a 48 refs/396 vs
   b 46/381, game b first); a `do { b->velX = dx; } while (0);` (debt: loop-depth weighted ref) puts b first again.
   Left (same as x001/wip/func_801244A0.c): r a1 vs a3, sabs result v0/v1, +0x4e8 lh a0/v1 swap, t/w block regs
   (game t = a2 untied from the b->h lhu, w = a1; int/unsigned short function-scope t, int w, w copy: no gain).
   Older: score 142 (was 146: dead `ay = q;` at the end keeps q alive past the r = q copy, restoring the game's move a3,v1;
   see x001/wip/func_801244A0.c, same source, for the block-local t/w form of the D_1F8001D2 block, which here
   scores worse (238) although it fixes the size). Older notes: remaining diffs are register choices only: q/r copy at +0xe4, ay abs at +0x158 (game negates the ay copy, gcc propagates cy), sabs result v0/v1, the by/ay temps at +0x578 and t/d at +0x604. Tried: int/short types of q/r/t/d/w, reversed compares, sabs forms; -fno-cse-skip-blocks not needed after the if/else sabs. */
// FUNC 8011e000 2276 X010
#include "TOBJ.H"

extern unsigned char D_8009D2C3;
extern unsigned char D_1F8001D2;
extern int rcos(int);
extern int rsin(int);

static __inline__ short sabs(short x)
{
    short r;
    if (x < 0) r = -x; else r = x;
    return r;
}

int func_8011E000(TObj *a, TObj *b, unsigned char mode, int ang)
{
    short dx, dy, cx, cy, ax, ay, s2, s7;
    short kind;
    short v;
    int q, r;

    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return 0;
    dx = a->h->p.whole - b->h->p.whole;
    ax = b->box0 + dx;
    if ((unsigned short)ax > b->box1) return 0;
    dy = a->y.p.whole - b->y.p.whole;
    ay = b->box0 + dy;
    if ((unsigned short)ay > b->box1) return 0;
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
        cx = (unsigned)(rcos(ang) * b->box0) >> 12;
        cy = (unsigned)(rsin(ang) * b->box0) >> 12;
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
        if (a->y.p.whole < b->y.p.whole - s2) {
            if ((-(s2 + 4) + b->y.p.whole) > a->y.p.whole + a->box2) return 3;
            if (D_8009D2C3 & 0x40) {
                if (D_1F8001D2 != 0) {
                    if (dx > 0) a->h->p.whole = b->h->p.whole + cx;
                    else if (b->subtype == 3) a->h->p.whole = b->h->p.whole + cx;
                    else a->h->p.whole = b->h->p.whole - cx;
                    return 3;
                }
                a->y.p.whole = (-(s2 + 4) + b->y.p.whole) - a->box2;
                a->y.p.frac = 0;
                a->b69 = 1;
                a->velY = 0;
                if (a->b9e != 0) return 3;
                b->velX = dx;
                b->b69 = 1;
                if ((unsigned)(ang - 0x401) < 0x400 || ang > 0xc00) {
                    v = (unsigned)(~ang + 1 & 0x3ff) >> 6;
                    if (v > 8) v = 8;
                } else {
                    v = (unsigned)(ang & 0x3ff) >> 6;
                    if (v > 8) v = 8;
                    v = -v;
                }
                a->wb0 = v;
            } else {
                a->y.p.whole = (-(s2 + 4) + b->y.p.whole) - a->box2;
                a->y.p.frac = 0;
                b->b69 = 1;
                if (dx > 0) b->b9d = 0;
                else b->b9d = 1;
            }
        } else {
            if (a->b04 == 1 && a->step < 2 && (D_8009D2C3 & 0x40) && sabs(dy) < 8) {
                if ((-(s2 + -4) + b->y.p.whole) < a->y.p.whole - (a->box3 - a->box2)) return 3;
                b->b69 = 1;
                b->velX = dx;
                a->y.p.whole = (-(s2 + -4) + b->y.p.whole) + (a->box3 - a->box2);
                if (dx < 0) a->h->p.whole -= s2;
                else a->h->p.whole += s2;
                return 3;
            }
            { int by = b->y.p.whole; if ((-(s2 + -4) + by) < a->y.p.whole - (a->box3 - a->box2)) return 3; }
            if (D_8009D2C3 & 0x40) {
                if (D_1F8001D2 != 0) {
                    short d = a->y.p.whole - ((-(s2 + -4) + b->y.p.whole) + (a->box3 - a->box2));
                    if (dx > 0) {
                        short t;
                        unsigned short w;
                        t = b->h->p.whole + cx;
                        w = t - a->h->p.whole + 2;
                        if (w >= 8) a->h->p.whole += d;
                        else a->h->p.whole = t;
                    } else {
                        short t;
                        unsigned short w;
                        t = b->h->p.whole - cx;
                        w = a->h->p.whole - t + 2;
                        if (w >= 8) a->h->p.whole -= d;
                        else a->h->p.whole = t;
                    }
                    return 3;
                }
                a->y.p.whole = (-(s2 + -4) + b->y.p.whole) + (a->box3 - a->box2);
                do { b->velX = dx; } while (0);
                b->b69 = 1;
                if (a->velY < 0) a->velY = 0;
            } else {
                a->y.p.whole = (-(s2 + -4) + b->y.p.whole) + (a->box3 - a->box2);
                b->b69 = 1;
                if (dx < 0) b->b9d = 0;
                else b->b9d = 1;
            }
        }
    } else {
        s2 = 0x10;
        if (a->b69 != 0 && sabs(dx) < 10) s2 = 4;
        if (b->h->p.whole + s7 < a->h->p.whole) {
            if (b->h->p.whole + s2 + s7 < a->h->p.whole - (a->box1 - a->box0)) return 3;
            if (D_8009D2C3 & 0x40) {
                b->velX = dx;
                b->b69 = 1;
                a->h->p.whole = s7 + (b->h->p.whole + s2) + (a->box1 - a->box0);
                if (a->velY > 0x300) a->velY = 0x300;
            } else {
                a->h->p.whole = s7 + (b->h->p.whole + s2) + (a->box1 - a->box0);
                b->b69 = 1;
                if ((unsigned short)ang > 0x800) {
                    if (dy < 0) b->b9d = 1;
                    else b->b9d = 0;
                } else {
                    if (dy > 0) b->b9d = 0;
                    else b->b9d = 1;
                }
            }
        } else {
            if (b->h->p.whole - s2 + s7 > a->h->p.whole + a->box0) return 3;
            if (D_8009D2C3 & 0x40) {
                a->h->p.whole = s7 + (b->h->p.whole - s2) - a->box0;
                b->velX = dx;
                b->b69 = 1;
                if (a->velY > 0x300) a->velY = 0x300;
            } else {
                a->h->p.whole = s7 + (b->h->p.whole - s2) - a->box0;
                b->b69 = 1;
                if ((unsigned short)ang > 0x800) {
                    if (dy > 0) b->b9d = 1;
                    else b->b9d = 0;
                } else {
                    if (dy < 0) b->b9d = 0;
                    else b->b9d = 1;
                }
            }
        }
    }
    ay = q;
}
