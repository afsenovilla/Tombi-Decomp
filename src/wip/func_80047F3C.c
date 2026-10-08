// FUNC 80047f3c 408 MAIN0
/* score 74: rewritten in func_800482EC style (TObj fields, short temps); code identical except register rotation: game o=t0, p=a2, px=a3 (ours o=a2, p=a3, px=t0). Local copies of o and decl orders do not change it.  b18: also tried inline wrapper (changes return tails), dy/hy/py vars like func_80048500, greedy short/int/ushort of all locals, per-access alias pointer for o and for p (greedy), void* or char* params with typed locals: all stay 74. b31: fixed a stray comment terminator that broke compilation; -dg shows global-alloc order o(17 refs/86 insns) > p > px(77) > t1(98); game needs p > px > o > t1, i.e. o's priority squeezed between px and t1 (inline wrapper: same rotation). */
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
    o->y.p.frac = 0;
    o->b69 = 1;
    o->y.p.whole = p->y.p.whole - (p->box2 + (o->box3 - o->box2));
    return 1;
}
