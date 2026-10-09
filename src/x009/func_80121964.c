// FUNC 80121964 244 X009
// MATCHING 80121964 244
#include "TOBJ.H"

typedef struct { char p0[8]; short w8; short wa; } X;

static __inline__ int snap(TObj *o, short t, short d)
{
    if (d < 5) o->h->p.whole = t;
    return 1;
}
static __inline__ int right(TObj *o, short h, short t)
{
    if (h < t) return snap(o, t, t - h);
    return 0;
}
static __inline__ int left(TObj *o, short h, short t)
{
    if (h > t) return snap(o, t, h - t);
    return 0;
}

int func_80121964(TObj *o)
{
    X *x = (X *)((char *)o + 0xb4);
    int r;
    if ((o->subtype & 0x7f) == 0) {
        TObj *p = (TObj *)o->d90;
        if (o->animFrame & 1) {
            o->wbc = p->a.p.whole - p->box0 + 0x14;
        } else {
            *(short *)&o->bbe = p->a.p.whole + (p->box1 - p->box0) - 0x14;
        }
    }
    if (o->animFrame & 1) r = right(o, o->a.p.whole, x->w8);
    else r = left(o, o->a.p.whole, x->wa);
    return r;
}
