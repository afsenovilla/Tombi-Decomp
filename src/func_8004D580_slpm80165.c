// VERSION SLPM_80165 slpm80165 8004D580 560
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of pushObjectOutOfObjectFacing (src/pushObjectOutOfObjectFacing.c, JP branch): only type 0x22 reacts
// to the landing (no type 2 case), and the scratchpad word 0x1F80019E is cleared last (also after a
// sideways push).
#include "TOBJ.H"
#define B0(p) ((unsigned short)(p)->box0)
#define B2(p) ((unsigned short)(p)->box2)

void func_8004D580(TObj *o, TObj *e)
{
    short ox;
    short w;
    short r;
    short v;
    short dx;
    short adx;
    short b1;

    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) >= 0x5b) return;
    if (o->animFrame & 1) ox = B0(o); else ox = (unsigned short)o->box1 - B0(o);
    w = B0(e) + ox;
    r = w;
    dx = o->h->p.whole - e->h->p.whole;
    adx = dx;
    if ((unsigned short)(dx + w) > e->box1 + (b1 = o->box1)) return;
    if ((unsigned short)(o->y.p.whole - e->y.p.whole + (B2(e) + (o->box3 - B2(o)))) > o->box3 + e->box3) return;
    v = r;
    if (dx < 0) {
        adx = -dx;
        r = -w;
    } else {
        if (o->animFrame & 1) ox = b1 - B0(o); else ox = B0(o);
        r = ox + ((unsigned short)e->box1 - B0(e));
        v = r;
    }
    if ((unsigned short)(v - adx) < 9) {
        if (o->b68) {
            e->b68 = o->b68;
            e->animFrame = o->animFrame & 1;
            o->b68 = 0;
        }
        o->h->p.whole = e->h->p.whole + r;
        if ((short)r < 0) o->b9d = 2;
        else o->b9d = 3;
        goto done;
    }
    if (o->b9c & 1) return;
    o->y.p.whole = e->y.p.whole - (B2(e) + (o->box3 - B2(o)));
    o->y.p.frac = 0;
    o->b69 = 1;
    if (o->category == 2 && o->type == 0x22) {
        o->active = 2;
        o->step = 5;
    }
done:
    *(short *)0x1F80019E = 0;
}
#endif
