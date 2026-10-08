// FUNC 80125994 336 X001
// MATCHING 80125994 336
#include "TOBJ.H"

extern short D_1F80019E;
extern unsigned short *D_8013C6A4[];
extern int func_800428C0(TObj *, TObj *);

void func_80125994(TObj *a, TObj *b)
{
    short dz, n;
    unsigned short t;
    unsigned short *p;

    if (b->active & 4) return;
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return;
    dz = a->y.p.whole - b->y.p.whole;
    t = dz + (b->box2 + a->box2);
    if ((unsigned short)t > a->box3 + b->box3) return;
    n = -dz - 0x18;
    p = D_8013C6A4[b->subtype];
    if (n < 0) n = 0;
    t = p[n >> 3];
    if ((unsigned short)(a->h->p.whole - (b->h->p.whole + t) + (b->box0 + a->box0)) > b->box1 + a->box1) return;
    if (func_800428C0(a, b)) b->animFrame = a->animFrame & 1;
    D_1F80019E = 0;
}
