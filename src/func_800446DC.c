// FUNC 800446dc 228 MAIN0
// MATCHING 800446dc 228
#include "TOBJ.H"
int func_800446DC(TObj *a, TObj *b)
{
    short w;
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return -1;
    if (b->animFrame & 1) w = b->box1 - b->box0;
    else w = b->box0;
    if ((unsigned short)(a->h->p.whole - b->h->p.whole + (w + a->box0)) > b->box1 + a->box1) return -1;
    if ((unsigned short)(a->y.p.whole - b->y.p.whole + (b->box2 + a->box2)) > a->box3 + b->box3) return -1;
    return 1;
}
