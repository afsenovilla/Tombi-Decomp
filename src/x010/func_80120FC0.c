// FUNC 80120fc0 244 X010
// MATCHING 80120fc0 244
#include "TOBJ.H"
extern int SquareRoot0(int);
extern int func_800428C0(TObj *, TObj *);

void func_80120FC0(TObj *o, TObj *p)
{
    short dx, dy;

    if ((unsigned short)(o->d->p.whole - p->d->p.whole + 0x2d) >= 0x5b) return;
    dx = o->h->p.whole - p->h->p.whole;
    if (p->box1 < (unsigned short)(p->box0 + dx)) return;
    dy = o->y.p.whole - p->y.p.whole;
    if (p->box3 < (unsigned short)(p->box2 + dy)) return;
    if (SquareRoot0(dx * dx + dy * dy) > p->box0) return;
    func_800428C0(o, p);
}
