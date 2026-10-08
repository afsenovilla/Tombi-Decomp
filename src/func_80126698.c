// FUNC 80126698 216 X000
// MATCHING 80126698 216
#include "TOBJ.H"
void func_80126698(TObj *a, TObj *b)
{
    short dx;
    b->b69 = 0;
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) < 0x5b) {
        dx = a->h->p.whole - b->h->p.whole;
        if ((unsigned short)(dx + (a->box0 + b->box0)) > a->box1 + b->box1) return;
        if ((unsigned short)(a->y.p.whole - b->y.p.whole + (a->box2 + b->box2)) > a->box3 + b->box3) return;
        if (dx > 0) b->b69 = 3;
        else b->b69 = 2;
        a->b69 = 2;
        *(short *)0x1F80019E = 0;
    }
}
