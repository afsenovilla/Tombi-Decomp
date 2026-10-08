// FUNC 801217f8 316 X009
// MATCHING 801217f8 316
#include "TOBJ.H"
extern short D_1F80019E;

void func_801217F8(TObj *a, TObj *b)
{
    short w, h;

    if (a->type != 0x21)
        return;
    if (a == b)
        return;
    if (b->active == 2)
        return;
    if (a->active == 2)
        return;
    if (a->w9a == 0)
        return;
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b)
        return;
    w = a->box1;
    if ((unsigned short)(a->h->p.whole - b->h->p.whole + (b->box0 + (w - a->box0))) > b->box1 + w)
        return;
    h = a->box3;
    if ((unsigned short)(a->y.p.whole - b->y.p.whole + (b->box2 + (h - a->box2))) > h + b->box3)
        return;
    b->active = 2;
    b->b04 = 2;
    b->step = 5;
    b->state = 0;
    a->active = 2;
    a->b04 = 2;
    a->step = 5;
    a->state = 0;
    D_1F80019E = 0;
}
