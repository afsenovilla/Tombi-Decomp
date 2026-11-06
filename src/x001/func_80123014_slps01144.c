// JP-only (x01), written from the disassembly
// VERSION SLPS_01144 x01 80123014 1504
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"

extern int func_8005FE94(int);
extern int func_8005FDC8(int);

static __inline__ short sabs(short x)
{
    short r;
    if (x < 0) r = -x; else r = x;
    return r;
}

int func_80123014_slps01144(TObj *a, TObj *b, unsigned char mode, int ang)
{
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
            if (SPAD.b1d2 != 0) {
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
                u = (unsigned)(~ang + 1 & 0x3ff) >> 6;
                if (u > 8) u = 8;
            } else {
                u = (unsigned)(ang & 0x3ff) >> 6;
                if (u > 8) u = 8;
                u = -u;
            }
            a->wb0 = u;
        } else {
            if ((-(s2 + -4) + b->y.p.whole) < a->y.p.whole - (a->box3 - a->box2)) return 3;
    asm("" :: "r"(dy));
            if (SPAD.b1d2 != 0) {
                short d = a->y.p.whole - ((-(s2 + -4) + b->y.p.whole) + (a->box3 - a->box2));
                if (dx > 0) {
                    short t;
                    t = b->h->p.whole + cx;
asm("" : "=r"(t) : "0"(t));
                    ay = t - a->h->p.whole + 2;
                    if ((unsigned short)ay >= 8) a->h->p.whole += d;
                    else a->h->p.whole = t;
                } else {
                    short t;
                    t = b->h->p.whole - cx;
asm("" : "=r"(t) : "0"(t));
                    ay = a->h->p.whole - t + 2;
                    if ((unsigned short)ay >= 8) a->h->p.whole -= d;
                    else a->h->p.whole = t;
                }
                return 3;
            }
            a->y.p.whole = (-(s2 + -4) + b->y.p.whole) + (a->box3 - a->box2);
            if (a->velY < 0) a->velY = 0;
        }
    } else {
        if (b->h->p.whole + s7 < a->h->p.whole) {
            if (b->h->p.whole + 12 + s7 < a->h->p.whole - (a->box1 - a->box0)) return 3;
            a->h->p.whole = s7 + (b->h->p.whole + 12) + (a->box1 - a->box0);
        } else {
            if (b->h->p.whole - 12 + s7 > a->h->p.whole + a->box0) return 3;
            a->h->p.whole = s7 + (b->h->p.whole - 12) - a->box0;
        }
        if (a->velY > 0x300) a->velY = 0x300;
    }
    ay = q;
}
#endif
