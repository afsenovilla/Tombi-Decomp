// VERSION PCPX_96084 tomba 8006B2A4 464
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of PAL x001 801251B0 (bendingBranchPlayerCollision.c): no 0x10 height offset, fields +0x98/+0xA8/+0xB6.
#include "TOBJ.H"

static __inline__ void coll(TObj *a, TObj *b)
{
    short s, dx, ax, w, off, d0, sy;
    int dy, d; unsigned short t;
    if ((unsigned short)(a->d->p.whole - b->d38 + 0x2d) > 0x5a) return;
    dx = a->h->p.whole - b->d30;
    ax = dx;
    s = b->box0 + a->box0;
    off = s;
    if ((unsigned short)(dx + s) > b->box1 + a->box1) return;
    d = -b->d34 + (unsigned short)a->y.p.whole;
    t = d + (b->box2 + a->box2);
    dy = d;
    if ((unsigned short)t > a->box3 + b->box3) return;
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
        a->h->p.whole = b->d30 + off;
        return;
    }
    sy = dy;
    if (sy <= 0) {
        if (*(unsigned char *)&a->w98 & 1) return;
        a->y.p.whole = b->d34 - (short)(b->box2 + a->box2);
        a->b69 = 1;
        a->y.p.frac = 0;
        if (d0 >= 0) {
            *(unsigned char *)&a->wb6 = 8;
            a->wa8 = 2;
        } else {
            *(unsigned char *)&a->wb6 = 9;
            a->wa8 = -2;
        }
        return;
    }
    a->y.p.whole = b->d34 + (short)((b->box3 - b->box2) + (a->box3 - a->box2));
    if (a->velY < 0) a->velY = 0;
}

void func_8006B2A4_pcpx96084(TObj *o, TObj *e)
{
    if (o->active != 3 && e->subtype == 6) coll(o, e);
}
#endif
