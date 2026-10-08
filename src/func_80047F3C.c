// FUNC 80047f3c 408 MAIN0
// MATCHING 80047f3c 408
#include "TOBJ.H"

int func_80047F3C(TObj *o, TObj *p)
{
    char pad;
    short dx, wx, px;
    short cx;

    if ((unsigned short)(o->d->p.whole - p->d->p.whole + 0x2d) >= 0x5b)
        return 0;
    wx = p->box0 + (o->box1 - o->box0);
    px = wx;
    dx = o->h->p.whole - p->h->p.whole;
    if ((unsigned short)(dx + wx) > p->box1 + o->box1)
        return 0;
    if ((unsigned short)((o->y.p.whole - p->y.p.whole) + (p->box2 + (o->box3 - o->box2))) > o->box3 + p->box3)
        return 0;
    cx = px;
    if (dx < 0) {
        dx = -dx;
        px = -px;
    } else {
        px = o->box0 + (p->box1 - p->box0);
        cx = px;
    }
    if ((unsigned short)(cx - dx) < 4) {
        o->h->p.whole = p->h->p.whole + px;
        if (px < 0)
            o->b9d = 2;
        else
            o->b9d = 3;
        return 2;
    }
    if (o->b9c & 1)
        return 0;
    {
        short w = p->y.p.whole - (p->box2 + (o->box3 - o->box2));
        TObj *q = o;
        o->y.p.frac = 0;
        o->b69 = 1;
        q->y.p.whole = w;
    }
    return 1;
}
