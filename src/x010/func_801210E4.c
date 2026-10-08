// FUNC 801210e4 496 X010
// MATCHING 801210e4 496
#include "TOBJ.H"
extern unsigned short D_8012F2F8[];

void func_801210E4(TObj *o, TObj *p)
{
    short dy, ny, k, d, e0, e1;
    unsigned short u, w;
    unsigned short a, b, h;
    unsigned short *tb;

    if ((unsigned short)(o->d->p.whole - p->d->p.whole + 0x2d) >= 0x5b)
        return;
    dy = o->y.p.whole - p->y.p.whole;
    u = dy + (p->box2 + o->box2);
    if ((unsigned short)u > o->box3 + p->box3)
        return;
    tb = D_8012F2F8;
    ny = -dy;
    k = ny - 0x18;
    if (k < 0) k = 0;
    u = tb[k >> 3];
    d = o->h->p.whole - (p->h->p.whole + u);
    a = p->box0;
    b = o->box0;
    w = a + b;
    e0 = p->box1;
    e1 = o->box1;
    if ((unsigned short)(d + w) > e0 + e1)
        return;
    if (d < 0) k = -w;
    else k = (e0 - a) + (e1 - b);
    h = p->box2 + o->box2;
    if (h - ny >= 9) {
        o->h->p.whole = p->h->p.whole + u + k;
        if (k < 0) o->ba6 = 2;
        else o->ba6 = 3;
        return;
    }
    if (o->b9c & 1) {
        o->h->p.whole = p->h->p.whole + u + k;
        return;
    }
    {
        short b = p->y.p.whole;
        o->y.p.frac = 0;
        o->b69 = 1;
        o->y.p.whole = b - h;
    }
    p->b69 = 1;
    if (k >= 0) {
        o->bbe = 8;
        o->wb0 = 2;
    } else {
        o->bbe = 9;
        o->wb0 = -2;
    }
}
