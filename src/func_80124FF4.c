// FUNC 80124ff4 708 X000
// MATCHING 80124ff4 708
#include "TOBJ.H"

#define LAND()                                          \
    v = e->d34;                                         \
    dy = v - (unsigned short)e->y.p.whole;              \
    v = e->d30;                                         \
    dx2 = e->h->p.whole - v;                            \
    dx = o->h->p.whole - v;                             \
    if (dx <= 0) {                                      \
        r = 0;                                          \
    } else if (dx2 < dx) {                              \
        r = dy;                                         \
    } else {                                            \
        r = dx * (short)dy / dx2;                       \
    }                                                   \
    t = r + e->box2;                                    \
    if (e->d34 - t > o->y.p.whole + o->box2) return 0;  \
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
    short dy, r;
    short t;
    int v;
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
