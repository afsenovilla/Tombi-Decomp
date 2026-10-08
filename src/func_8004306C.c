// FUNC 8004306c 500 MAIN0
// MATCHING 8004306c 500
#include "TOBJ.H"

int func_8004306C(TObj *o, TObj *e)
{
    short dx, dy, sx, sy, px, cx, cy;
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 45) > 90)
        return 0;
    px = sx = e->box0 + o->box0;
    dx = o->h->p.whole - e->h->p.whole;
    if ((unsigned short)(dx + sx) > e->box1 + o->box1)
        return 0;
    dy = o->y.p.whole - e->y.p.whole;
    sy = e->box2 + o->box2;
    if ((unsigned short)(dy + sy) > o->box3 + e->box3)
        return 0;
    cx = px;
    if (dx < 0) {
        dx = -dx;
        px = -sx;
    } else {
        px = (e->box1 - e->box0) + (o->box1 - o->box0);
        cx = px;
    }
    cy = sy;
    if (dy < 0) {
        dy = -dy;
        sy = -sy;
    } else {
        sy = (e->box3 - e->box2) + (o->box3 - o->box2);
        cy = sy;
    }
    if (cx - dx < cy - dy || sy > 0) {
        o->h->p.whole = e->h->p.whole + px;
        if (px < 0) o->ba6 = 2;
        else o->ba6 = 3;
        return 2;
    }
    if (o->b9c & 1) return 0;
    o->wb0 = 0;
    o->y.p.whole = e->y.p.whole - (e->box2 + o->box2);
    o->y.p.frac = 0;
    o->b69 = 1;
    o->velY = 0;
    e->b69 = 1;
    return 1;
}
