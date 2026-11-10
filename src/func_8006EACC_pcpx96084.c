// VERSION PCPX_96084 tomba 8006EACC 380
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of pushCat2ObjectOutOfType04 (src/pushCat2ObjectOutOfType04.c): side result in +0x69, no flag test before landing.
#include "TOBJ.H"

int func_8006EACC_pcpx96084(TObj *o, TObj *p)
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
            o->b69 = 2;
        else
            o->b69 = 3;
        return 2;
    }
    {
        short w = p->y.p.whole - (p->box2 + (o->box3 - o->box2));
        TObj *q = o;
        o->y.p.frac = 0;
        o->b69 = 1;
        q->y.p.whole = w;
    }
    return 1;
}
#endif
