// FUNC 800480d4 536 MAIN0
// r9 wip: score 109. Sibling of func_800482EC. Game recomputes hy+dy for the <0xc test (no CSE, plain addu/andi/sltiu) and keeps o/p in t0/t1 (more pseudos).
#include "TOBJ.H"

int func_800480D4(TObj *o, TObj *p)
{
    char pad;
    short dx, px;
    int wx;
    short dy; int hy;
    short cx, ax, ay;

    if ((unsigned short)(o->d->p.whole - p->d->p.whole + 0x2d) >= 0x5b)
        return 0;
    wx = (short)(p->box0 + (o->box1 - o->box0));
    px = wx;
    dx = o->h->p.whole - p->h->p.whole;
    ax = dx;
    if ((unsigned short)(dx + wx) > p->box1 + o->box1)
        return 0;
    dy = o->y.p.whole - p->y.p.whole;
    hy = p->box2 + (o->box3 - o->box2);
    ay = dy;
    if ((unsigned short)(dy + hy) > o->box3 + p->box3)
        return 0;
    if (((unsigned)(hy + dy) & 0xffff) < 0xc) {
        o->y.p.whole = p->y.p.whole - hy;
        o->y.p.frac = 0;
        o->b69 = 1;
        return 1;
    }
    cx = px;
    if (dx < 0) {
        ax = -dx;
        px = -px;
    } else {
        px = o->box0 + (p->box1 - p->box0);
        cx = px;
    }
    if ((unsigned short)(cx - ax) < 4) {
        o->h->p.whole = p->h->p.whole + px;
        if (px < 0)
            o->b9d = 2;
        else
            o->b9d = 3;
        return 2;
    }
    if (ay <= 0) {
        if (o->b9c & 1)
            return 0;
        o->y.p.frac = 0;
        o->b69 = 1;
        o->y.p.whole = p->y.p.whole - (p->box2 + (o->box3 - o->box2));
        return 1;
    }
    if (o->category == 2 && *(unsigned short *)&o->b04 == 0x102)
        return 0;
    o->y.p.whole = p->y.p.whole + (o->box2 + (p->box3 - p->box2));
    return 3;
}
