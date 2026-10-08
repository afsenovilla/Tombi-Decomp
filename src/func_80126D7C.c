// FUNC 80126d7c 492 X000
// MATCHING 80126d7c 492
#include "TOBJ.H"
#define B(o, n) (((unsigned char *)(o))[n])
void FUN_8004886c(TObj *o, TObj *e);

void func_80126D7C(TObj *o, TObj *e)
{
    short sx, dx, w, a, t; int dy; int d; unsigned short u;
    if (e->subtype != 2) {
        FUN_8004886c(o, e);
        return;
    }
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) >= 0x5b)
        return;
    if (o->animFrame & 1)
        sx = o->box0;
    else
        sx = o->box1 - o->box0;
    dx = o->h->p.whole - e->h->p.whole;
    w = e->box0 + sx;
    if ((unsigned short)dx > e->box1 + o->box1)
        return;
    d = (unsigned short)o->y.p.whole - (unsigned short)e->y.p.whole;
    t = o->box3;
    u = d + (e->box2 + (t - o->box2));
    dy = d;
    if (u > t + e->box3)
        return;
    if (dx < 0) {
        a = -dx;
        dx = -w;
        if ((unsigned short)(w - a) < 4) {
            if (w == a)
                return;
            o->h->p.whole = e->h->p.whole + dx;
            B(o, 0x9d) = 2;
            return;
        }
    }
    if ((short)dy <= 0) {
        if (o->b9c & 1)
            return;
        o->y.p.whole = e->y.p.whole - (e->box2 + (o->box3 - o->box2));
        o->y.p.frac = 0;
        o->b69 = 1;
        return;
    }
    if (o->category == 2 && *(unsigned short *)&o->b04 == 0x102)
        return;
    o->y.p.whole = e->y.p.whole + (o->box2 + (e->box3 - e->box2));
}
