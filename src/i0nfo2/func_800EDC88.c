// FUNC 800edc88 612 I0NFO2
// MATCHING 800edc88 612
#include "TOBJ.H"

void func_800EDC88(TObj *a, TObj *b)
{
    short dx, s, dy, t, w, h;
    unsigned short u;
    dx = a->a.p.whole - b->a.p.whole;
    s = b->box0 + a->box0;
    if ((unsigned short)(dx + s) > b->box1 + a->box1) return;
    if (b->b9c == 1) {
        dy = a->y.p.whole - b->y.p.whole;
        t = b->box2 + a->box2 + 8;
        u = dy + t;
        if (u > a->box3 + b->box3 + 0x10) return;
    } else {
        dy = a->y.p.whole - b->y.p.whole;
        t = b->box2 + a->box2;
        u = dy + t;
        if (u > a->box3 + b->box3) return;
    }
    w = s;
    if (dx < 0) {
        dx = -dx;
        s = -s;
    } else {
        s = (b->box1 - b->box0) + (a->box1 - a->box0);
        w = s;
    }
    h = t;
    if (dy < 0) {
        dy = -dy;
        t = -t;
    } else {
        t = (b->box3 - b->box2) + (a->box3 - a->box2);
        h = t;
    }
    if (b->b9c == 1 || w - dx < h - dy) {
        a->a.p.whole = b->a.p.whole + s;
        if (s < 0) {
            a->b69 = 1;
            b->b69 = 3;
        } else {
            a->b69 = 3;
            b->b69 = 1;
        }
    } else if (t <= 0) {
        a->y.p.whole = b->y.p.whole + t;
        a->y.p.frac = 0;
        a->b69 = 4;
        b->b69 = 2;
    } else {
        a->y.p.whole = b->y.p.whole + t;
        a->y.p.frac = 0;
        a->b69 = 2;
        b->b69 = 4;
    }
    if (!(b->active & 2) && *(unsigned char *)&a->wac == 1 && a->b69 == ((a->animFrame & 7) >> 1) + 1) {
        *(unsigned char *)&a->wac = 2;
        b->b68 = 1;
        a->d90 = (int)b;
        *(short *)0x1F80019E = 0;
    }
}
