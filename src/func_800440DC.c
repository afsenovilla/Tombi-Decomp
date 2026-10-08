// FUNC 800440dc 224 MAIN0
// MATCHING 800440dc 224
#include "TOBJ.H"

static __inline__ short hit(TObj *a, TObj *b)
{
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return 0;
    if ((unsigned short)(a->h->p.whole - b->h->p.whole + (a->box0 + b->box0)) > a->box1 + b->box1) return 0;
    if ((unsigned short)(a->y.p.whole - b->y.p.whole + (a->box2 + b->box2)) > a->box3 + b->box3) return 0;
    return 1;
}

void func_800440DC(TObj *a, TObj *b)
{
    b->b69 = 0;
    if (hit(a, b)) {
        if (b->type != 0x26) {
            *(unsigned char *)&a->da0 = 4;
        }
        b->b69 = 1;
    }
}
