// FUNC 801244a0 1800 X001
/* score 134 (1808 B vs 1800): ported from x010/wip/func_8011E000.c (same code without the D_8009D2C3 & 0x40
   tests, and the b->velX/b69 stores only in the first landing branch). Same residue as the x010 sibling:
   ang/cx in s6/s5 (game s5/s6), r = q coalesced (game `move a3,v1` copy), cy copy propagated into the abs,
   sabs v0/v1, a->y/b->y a0/v1 swap at +0x4e8, and in the D_1F8001D2 block t is a copy of the sum (2 extra
   moves; game keeps t in a2 and (t - a->h + 2) in a1). Tried: q/r types (16 combos) and shapes, d/t/w types
   (18 combos), declaration order of the shorts (no effect). */
#include "TOBJ.H"

extern unsigned char D_1F8001D2;
extern int rcos(int);
extern int rsin(int);

static __inline__ short sabs(short x)
{
    short r;
    if (x < 0) r = -x; else r = x;
    return r;
}

int func_801244A0(TObj *a, TObj *b, unsigned char mode, int ang)
{
    short dx, dy, cx, cy, ax, ay, s2, s7;
    short kind;
    short t;
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
            if (D_1F8001D2 != 0) {
                if (dx > 0) a->h->p.whole = b->h->p.whole + cx;
                else a->h->p.whole = b->h->p.whole - cx;
                return 3;
            }
            a->y.p.whole = (-(s2 + 4) + b->y.p.whole) - a->box2;
            a->y.p.frac = 0;
            a->velY = 0;
            a->b69 = 1;
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
            if (a->b04 == 1 && a->step < 2 && sabs(dy) < 8) {
                if ((-(s2 + -4) + b->y.p.whole) < a->y.p.whole - (a->box3 - a->box2)) return 3;
                a->y.p.whole = (-(s2 + -4) + b->y.p.whole) + (a->box3 - a->box2);
                if (dx < 0) a->h->p.whole -= s2;
                else a->h->p.whole += s2;
                return 3;
            }
            if ((-(s2 + -4) + b->y.p.whole) < a->y.p.whole - (a->box3 - a->box2)) return 3;
            if (D_1F8001D2 != 0) {
                short d = a->y.p.whole - ((-(s2 + -4) + b->y.p.whole) + (a->box3 - a->box2));
                short t;
                if (dx > 0) {
                    t = b->h->p.whole + cx;
                    if ((unsigned short)(t - a->h->p.whole + 2) >= 8) a->h->p.whole += d;
                    else a->h->p.whole = t;
                } else {
                    t = b->h->p.whole - cx;
                    if ((unsigned short)(a->h->p.whole - t + 2) >= 8) a->h->p.whole -= d;
                    else a->h->p.whole = t;
                }
                return 3;
            }
            a->y.p.whole = (-(s2 + -4) + b->y.p.whole) + (a->box3 - a->box2);
            if (a->velY < 0) a->velY = 0;
        }
    } else {
        s2 = 0x10;
        if (a->b69 != 0 && sabs(dx) < 10) s2 = 4;
        if (b->h->p.whole + s7 < a->h->p.whole) {
            if (b->h->p.whole + s2 + s7 < a->h->p.whole - (a->box1 - a->box0)) return 3;
            a->h->p.whole = s7 + (b->h->p.whole + s2) + (a->box1 - a->box0);
            if (a->velY > 0x300) a->velY = 0x300;
        } else {
            if (b->h->p.whole - s2 + s7 > a->h->p.whole + a->box0) return 3;
            a->h->p.whole = s7 + (b->h->p.whole - s2) - a->box0;
            if (a->velY > 0x300) a->velY = 0x300;
        }
    }
}
