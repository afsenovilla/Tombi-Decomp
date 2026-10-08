// FUNC 80124ff4 708 X000
/* score 141: structure/layout right (LAND duplicated as macro, return-0 shared). Remaining: land section regalloc/sched: game keeps dy in t0 with copy a3 (move a3,t0 in bgtz delay), dx2 short-extended lazily, quotient copied (mflo v0; move v1,v0); inline/int-dx2/copies tried */
#include "TOBJ.H"

#define LAND()                                          \
    dy = e->d34 - (unsigned short)e->y.p.whole;         \
    dx2 = e->h->p.whole - e->d30;                       \
    dx = o->h->p.whole - e->d30;                        \
    if (dx <= 0) {                                      \
        r = 0;                                          \
    } else if (dx2 < dx) {                              \
        r = dy;                                         \
    } else {                                            \
        r = dx * (short)dy / dx2;                       \
    }                                                   \
    t = r + e->box2;                                    \
    if (o->y.p.whole + o->box2 < e->d34 - t) return 0;  \
    o->y.p.whole = e->d34 - t - o->box2;                \
    o->y.p.frac = 0;                                    \
    o->velY = 0;                                        \
    o->b69 = 1;                                         \
    e->b69 = 1;                                         \
    return 1;

int func_80124FF4(TObj *o, TObj *e)
{
    unsigned short dy0;
    short dx, dx2;
    int dy, r;
    short t;
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) > 0x5a) return 0;
    if ((unsigned short)(o->h->p.whole - e->h->p.whole + (e->box0 + o->box0)) > e->box1 + o->box1) return 0;
    dy0 = o->y.p.whole - e->y.p.whole;
    if ((unsigned short)(dy0 + (e->box2 + o->box2)) > o->box3 + e->box3) return 0;
    if (e->subtype == 2) {
        LAND()
    }
    if ((short)dy0 <= 0) {
        LAND()
    }
    o->y.p.whole = e->y.p.whole + ((e->box3 - e->box2) + (o->box3 - o->box2));
    if (o->velY < 0) o->velY = 0;
    return 3;
}
