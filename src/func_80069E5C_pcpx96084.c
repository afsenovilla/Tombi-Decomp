// VERSION PCPX_96084 tomba 80069E5C 640
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL pushObjectOutOfObjectBoxC (src/pushObjectOutOfObjectBoxC.c): fields +0x98/+0xA2/+0xA8/+0xB6 (PAL +0x9C/+0xA6/+0xB0/+0xBE).
#include "TOBJ.H"

int pushObjectOutOfObjectBoxC(TObj *a, TObj *b)
{
    short s, dx, ax, w, off, d0, sy;
    int dy, d; unsigned short t;
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) > 0x5a) return 0;
    s = b->box0 + a->box0;
    dx = a->h->p.whole - b->h->p.whole;
    off = s;
    ax = dx;
    if ((unsigned short)(dx + s) > b->box1 + a->box1) return 0;
    d = (unsigned short)a->y.p.whole - (unsigned short)b->y.p.whole;
    t = d + (b->box2 + a->box2);
    dy = d;
    if ((unsigned short)t > a->box3 + b->box3) return 0;
    w = off;
    d0 = ax;
    if (dx < 0) {
        ax = -dx;
        off = -s;
    } else {
        off = (b->box1 - b->box0) + (a->box1 - a->box0);
        w = off;
    }
    if ((unsigned short)(w - ax) < 4) {
        a->h->p.whole = b->h->p.whole + off;
        if (off < 0) (*((unsigned char *)&a->da0 + 2)) = 2; else (*((unsigned char *)&a->da0 + 2)) = 3;
        return 2;
    }
    sy = dy;
    if (sy <= 0) {
        if ((*(unsigned char *)&a->w98) & 1) {
            a->h->p.whole = b->h->p.whole + off;
            return 2;
        }
        a->y.p.whole = b->y.p.whole - (b->box2 + a->box2);
        a->y.p.frac = 0;
        a->b69 = 1;
        b->b69 = 1;
        if (d0 >= 0) {
            (*(unsigned char *)&a->wb6) = 8;
            (a->wa8) = 2;
        } else {
            (*(unsigned char *)&a->wb6) = 9;
            (a->wa8) = -2;
        }
        return 1;
    }
    if ((b->box3 - b->box2) + (a->box3 - a->box2) - sy < 5) {
        a->y.p.whole = b->y.p.whole + ((b->box3 - b->box2) + (a->box3 - a->box2));
        if (a->velY < 0) {
            a->velY = 0;
            b->b69 = 1;
            if (b->type == 5) b->b69 = 4;
        }
        return 3;
    }
    a->h->p.whole = b->h->p.whole + off;
    if (off < 0) (*((unsigned char *)&a->da0 + 2)) = 2; else (*((unsigned char *)&a->da0 + 2)) = 3;
    return 2;
}
#endif
