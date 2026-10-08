// FUNC 800480d4 536 MAIN0
// wip score 107 (rewritten in the style of the matched sibling func_800482EC; int hy). Left: game keeps o/p in t0/t1 (ours a2/a3: two fewer early pseudos) and loads o->box3 with lhu (sll/sra later). As a static inline wrapper: 142.
#include "TOBJ.H"

int func_800480D4(TObj *o, TObj *p)
{
    char pad;
    short dx;
    short wx;
    short px;
    short dy;
    int hy;
    short cx;

    if ((unsigned short)(o->d->p.whole - p->d->p.whole + 0x2d) >= 0x5b)
        return 0;
    wx = p->box0 + (o->box1 - o->box0);
    px = wx;
    dx = o->h->p.whole - p->h->p.whole;
    if ((unsigned short)(dx + wx) > p->box1 + o->box1)
        return 0;
    dy = o->y.p.whole - p->y.p.whole;
    hy = p->box2 + (o->box3 - o->box2);
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
    if (dy <= 0) {
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
