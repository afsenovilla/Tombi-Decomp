// FUNC 80126d7c 492 X000
#include "TOBJ.H"
#define B(o, n) (((unsigned char *)(o))[n])
void FUN_8004886c(TObj *o, TObj *e);

void func_80126D7C(TObj *o_, TObj *e_)
{
    TObj *e = e_;
    TObj *o = o_;
    short sx, dx, dy, w, a;
    if (e->subtype != 2) {
        FUN_8004886c(o_, e_);
        return;
    }
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 45) > 90)
        return;
    if (o->animFrame & 1)
        sx = o->box0;
    else
        sx = o->box1 - o->box0;
    dx = o->h->p.whole - e->h->p.whole;
    w = e->box0 + sx;
    if ((unsigned short)dx > e->box1 + o->box1)
        return;
    dy = o->y.p.whole - e->y.p.whole;
    if ((unsigned short)(dy + (e->box2 + (o->box3 - o->box2))) > o->box3 + e->box3)
        return;
    if (dx < 0) {
        a = -dx;
        if ((unsigned short)(w - a) < 4) {
            dx = -w;
            if (w == a)
                return;
            o->h->p.whole = e->h->p.whole + dx;
            B(o, 0x9d) = 2;
            return;
        }
    }
    if (dy <= 0) {
        if (o->b9c & 1)
            return;
        o->y.p.frac = 0;
        o->b69 = 1;
        o->y.p.whole = e->y.p.whole - (e->box2 + (o->box3 - o->box2));
    } else {
        if (o->category == 2 && *(unsigned short *)&o->b04 == 0x102)
            return;
        o->y.p.whole = e->y.p.whole + (o->box2 + (e->box3 - e->box2));
    }
}
