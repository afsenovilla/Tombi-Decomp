// FUNC 801251b0 476 X001
// MATCHING 801251b0 476
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
    d = (unsigned short)a->y.p.whole - 0x10;
    d -= b->d34;
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
        if (a->b9c & 1) return;
        a->y.p.whole = b->d34 - (short)((b->box2 + a->box2) - 0x10);
        a->b69 = 1;
        a->y.p.frac = 0;
        if (d0 >= 0) {
            a->bbe = 8;
            a->wb0 = 2;
        } else {
            a->bbe = 9;
            a->wb0 = -2;
        }
        return;
    }
    a->y.p.whole = b->d34 + (short)((b->box3 - b->box2) + (a->box3 - a->box2) + 0x10);
    if (a->velY < 0) a->velY = 0;
}

void func_801251B0(TObj *o, TObj *e)
{
    if (o->active != 4 && e->subtype == 6) coll(o, e);
}
