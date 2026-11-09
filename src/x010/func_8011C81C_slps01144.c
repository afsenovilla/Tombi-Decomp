// VERSION SLPS_01144 x10 8011C81C 1868
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
/* Matched. Near-twin of PAL collidePlayerWithTiltedBar (x010/func_8011E000.c): JP drops the subtype==3 case, the b04 ride
   block and the velX/b69 stores of the lower-surface path, uses a constant 12 instead of s2 in the side arms and tests
   D_8009EE6B & 0x40 first in the last arm. What fixed the last 26 lines:
   - the `asm("" :: "r"(dx)); asm("" :: "r"(ang));` keep-alives go before `ax = cx;`, not before `switch (kind)`: reorg
     cannot fill a delay slot past an asm insn, and there they sat at the `bnez ay` target (retail steals `move v1,a2`);
   - the PAL keep-alive of v across the wb0 else arm (`asm("" : "=r"(v));` ... `asm("" :: "r"(v));`) gives v a v0
     conflict, and a second `asm("" : "=r"(t) : "0"(t));` per SPAD.b1d2 arm raises t's global-alloc priority above v
     (floor_log2(refs)*refs/live_length), so t takes v1 first, v skips a0 (preferred by the a->h pointer) and gets a1,
     a->h value a2, d a3, and the wb0 u keeps v1. */
#include "TOBJ.H"

extern unsigned char D_8009EE6B;
extern int func_8005FE94(int);
extern int func_8005FDC8(int);

static __inline__ short sabs(short x)
{
    short r;
    if (x < 0) r = -x; else r = x;

    return r;
}

int func_8011C81C(TObj *a, TObj *b, unsigned char mode, int ang0)
{
    int ang = ang0;
    short dx, dy, cx, cy, ax, ay, s2, s7;
    short kind;
    short v;
    short u;
    int q, r, c1;

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
        asm("" ::: "$5", "$6");
        if ((unsigned)(((ang - r) & 0xfff) - 0x500) > 0x900) return 1;
    case 0:
        do { c1 = func_8005FE94(ang); } while (0);
        cx = (unsigned)(c1 * b->box0) >> 12;
        cy = (unsigned)(func_8005FDC8(ang) * b->box0) >> 12;
    }
    asm("" :: "r"(dx));
    asm("" :: "r"(ang));
    ax = cx;
    asm("" : "=r"(cx) : "0"(cx));
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
            if (D_8009EE6B & 0x40) {
                if (SPAD.b1d2 != 0) {
                    if (dx > 0) a->h->p.whole = b->h->p.whole + cx;
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
                    u = (unsigned)(~ang + 1 & 0x3ff) >> 6;
                    if (u > 8) u = 8;
                } else {
                    u = (unsigned)(ang & 0x3ff) >> 6;
                    asm("" : "=r"(v));
                    if (u > 8) u = 8;
                    u = -u;
                    asm("" :: "r"(v));
                }
            a->wb0 = u;
            } else {
                a->y.p.whole = (-(s2 + 4) + b->y.p.whole) - a->box2;
                a->y.p.frac = 0;
                b->b69 = 1;
                if (dx > 0) b->b9d = 0;
                else b->b9d = 1;
            }
        } else {
            { int by = b->y.p.whole; if ((-(s2 + -4) + by) < a->y.p.whole - (a->box3 - a->box2)) return 3; }
            if (D_8009EE6B & 0x40) {
                if (SPAD.b1d2 != 0) {
                    short d = a->y.p.whole - ((-(s2 + -4) + b->y.p.whole) + (a->box3 - a->box2));
                    if (dx > 0) {
                        short t;
                        t = b->h->p.whole + cx;
                        asm("" : "=r"(t) : "0"(t));
                        asm("" : "=r"(t) : "0"(t));
                        v = t - a->h->p.whole + 2;
                        if ((unsigned short)v >= 8) a->h->p.whole += d;
                        else a->h->p.whole = t;
                    } else {
                        short t;
                        t = b->h->p.whole - cx;
                        asm("" : "=r"(t) : "0"(t));
                        asm("" : "=r"(t) : "0"(t));
                        v = a->h->p.whole - t + 2;
                        if ((unsigned short)v >= 8) a->h->p.whole -= d;
                        else a->h->p.whole = t;
                    }
                    return 3;
                }
                a->y.p.whole = (-(s2 + -4) + b->y.p.whole) + (a->box3 - a->box2);
                if (a->velY < 0) a->velY = 0;
            } else {
                a->y.p.whole = (-(s2 + -4) + b->y.p.whole) + (a->box3 - a->box2);
                b->b69 = 1;
                if (dx < 0) b->b9d = 0;
                else b->b9d = 1;
            }
        }
    } else {
        if (b->h->p.whole + s7 < a->h->p.whole) {
            if (b->h->p.whole + 12 + s7 < a->h->p.whole - (a->box1 - a->box0)) return 3;
            if (D_8009EE6B & 0x40) {
                a->h->p.whole = s7 + (b->h->p.whole + 12) + (a->box1 - a->box0);
                if (a->velY > 0x300) a->velY = 0x300;
            } else {
                a->h->p.whole = s7 + (b->h->p.whole + 12) + (a->box1 - a->box0);
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
            if (D_8009EE6B & 0x40) {
                if (b->h->p.whole - 12 + s7 > a->h->p.whole + a->box0) return 3;
                a->h->p.whole = s7 + (b->h->p.whole - 12) - a->box0;
                if (a->velY > 0x300) a->velY = 0x300;
            } else {
                if (b->h->p.whole - 12 + s7 > a->h->p.whole + a->box0) return 3;
                a->h->p.whole = s7 + (b->h->p.whole - 12) - a->box0;
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
#endif
