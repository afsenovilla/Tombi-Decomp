// FUNC 80121964 244 X009
/* score 29: frame 0x10 vs 0x20 (game has inline-style param copies move a2,v0/move a0,v1); tried int/short locals, snap/right/left inlines */
#include "TOBJ.H"

typedef struct { char p0[8]; short w8; short wa; } X;

static __inline__ int snap(TObj *o, short t, short d)
{
    if (d < 5) o->h->p.whole = t;
    return 1;
}

int func_80121964(TObj *o)
{
    X *x = (X *)((char *)o + 0xb4);
    short h;
    int t;

    if ((o->subtype & 0x7f) == 0) {
        TObj *p = (TObj *)o->d90;
        if (o->animFrame & 1) {
            o->wbc = p->a.p.whole - p->box0 + 0x14;
        } else {
            *(short *)&o->bbe = p->a.p.whole + (p->box1 - p->box0) - 0x14;
        }
    }
    if (o->animFrame & 1) {
        h = o->a.p.whole;
        t = x->w8;
        if (h < t) return snap(o, t, t - h);
    } else {
        h = o->a.p.whole;
        t = x->wa;
        if (t < h) return snap(o, t, h - t);
    }
    return 0;
}
