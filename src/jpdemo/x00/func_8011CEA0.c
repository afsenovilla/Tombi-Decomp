// JPDEMO x00 8011CEA0 540
// Demo version of PAL x000 80126D7C (pushObjectOutOfCat4Type06.c): clears scratchpad 0x1F80019E on a hit.
#include "TOBJ.H"
#define B(o, n) (((unsigned char *)(o))[n])
short pushObjectOutOfObjectFacingB(TObj *o, TObj *e);

extern short D_1F80019E;
void pushObjectOutOfCat4Type06(TObj *o, TObj *e)
{
    short sx, dx, w, a, t; int dy; int d; unsigned short u;
    if (e->subtype != 2) {
        if (pushObjectOutOfObjectFacingB(o, e)) D_1F80019E = 0;
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
            D_1F80019E = 0;
            return;
        }
    }
    if ((short)dy <= 0) {
        if (o->b9c & 1)
            return;
        o->y.p.whole = e->y.p.whole - (e->box2 + (o->box3 - o->box2));
        o->y.p.frac = 0;
        o->b69 = 1;
        D_1F80019E = 0;
        return;
    }
    if (o->category == 2 && *(unsigned short *)&o->b04 == 0x102)
        return;
    o->y.p.whole = e->y.p.whole + (o->box2 + (e->box3 - e->box2));
}
