// FUNC 80048500 588 MAIN0
// MATCHING 80048500 588
#include "TOBJ.H"

int func_80048500(TObj *o, TObj *p)
{
    char pad;
    short dx, wx, px;
    short dy, hy, py;
    short cx, cy;

    if ((unsigned short)(o->d->p.whole - p->d->p.whole + 0x2d) >= 0x5b)
        return 0;
    wx = p->box0 + (o->box1 - o->box0);
    px = wx;
    dx = o->h->p.whole - p->h->p.whole;
    if ((unsigned short)(dx + wx) > p->box1 + o->box1)
        return 0;
    dy = o->y.p.whole - p->y.p.whole;
    hy = p->box2 + (o->box3 - o->box2);
    py = hy;
    if ((unsigned short)(dy + hy) > o->box3 + p->box3)
        return 0;
    cx = px;
    if (dx < 0) {
        dx = -dx;
        px = -px;
    } else {
        px = o->box0 + (p->box1 - p->box0);
        cx = px;
    }
    cy = py;
    if (dy < 0) {
        dy = -dy;
        py = -py;
    } else {
        py = o->box2 + (p->box3 - p->box2);
        cy = py;
    }
    if (cx - dx < cy - dy) {
        o->h->p.whole = p->h->p.whole + px;
        if (px < 0)
            o->b9d = 2;
        else
            o->b9d = 3;
        return 2;
    }
    if (py <= 0) {
        if (o->b9c & 1)
            return 0;
        if (px < 0)
            o->h->p.whole -= 1;
        else
            o->h->p.whole += 1;
        o->y.p.whole = p->y.p.whole + py;
        o->y.p.frac = 0;
        o->b69 = 1;
        return 1;
    }
    if (o->category == 2 && *(unsigned short *)&o->b04 == 0x102)
        return 0;
    o->y.p.whole = p->y.p.whole + py;
    return 3;
}
