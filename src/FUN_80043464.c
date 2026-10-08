// FUNC 80043464 380 MAIN0
// MATCHING 80043464 380
#include "TOBJ.H"

int FUN_80043464(TObj *o, TObj *e)
{
    short dx, dy, sx, sy, ax, ay;
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 45) > 90)
        return -1;
    sx = e->box0 + o->box0;
    dx = o->h->p.whole - e->h->p.whole;
    if ((unsigned short)(dx + sx) > e->box1 + o->box1)
        return -1;
    dy = o->y.p.whole - e->y.p.whole;
    sy = e->box2 + o->box2;
    if ((unsigned short)(dy + sy) > o->box3 + e->box3)
        return -1;
    ax = dx;
    if (dx < 0)
        dx = -dx;
    else
        sx = (e->box1 - e->box0) + (o->box1 - o->box0);
    ay = dy;
    if (dy < 0)
        dy = -dy;
    else
        sy = (e->box3 - e->box2) + (o->box3 - o->box2);
    if (sx - dx < sy - dy)
        return ax >= 0;
    if (ay <= 0)
        return 3;
    return 2;
}
