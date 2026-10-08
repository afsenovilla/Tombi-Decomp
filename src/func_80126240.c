// FUNC 80126240 548 X000
// MATCHING 80126240 548
#include "TOBJ.H"

void func_80126240(TObj *a, TObj *b)
{
    short dx, adx, px, wx, dy, py, wy;
    short s0, s2;

    if (a->b9e == 5) return;
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return;
    s0 = b->box0 + a->box0;
    px = s0;
    dx = a->h->p.whole - b->h->p.whole;
    adx = dx;
    if ((unsigned short)(dx + s0) > b->box1 + a->box1) return;
    dy = a->y.p.whole - b->y.p.whole;
    py = b->box2 + a->box2;
    if ((unsigned short)(dy + py) > a->box3 + b->box3) return;
    wx = px;
    if (dx < 0) {
        adx = -dx;
        px = -s0;
    } else {
        px = (b->box1 - b->box0) + (a->box1 - a->box0);
        wx = px;
    }
    wy = py;
    if (dy < 0) {
        dy = -dy;
        py = -py;
    } else {
        py = (b->box3 - b->box2) + (a->box3 - a->box2);
        wy = py;
    }
    if (wx - adx < wy - dy) {
        a->h->p.whole = b->h->p.whole + px;
        if (px < 0) goto two;
        a->ba6 = 3;
    } else if (py <= 0) {
        unsigned char k = 1;
        if (a->b9c & 1) {
        a->h->p.whole = b->h->p.whole + px;
    two:
        a->ba6 = 2;
        } else {
        short by = b->y.p.whole;
        a->bbe = 9;
        a->animFrame = 1;
        a->y.p.frac = 0;
        a->b69 = k;
        a->wb0 = -2;
        a->y.p.whole = by + py;
        b->b69 = k;
        }
    } else {
        a->y.p.whole = b->y.p.whole + py;
        if (a->velY < 0) a->velY = 0;
    }
}
