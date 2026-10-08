// FUNC 801213f0 324 X010
// MATCHING 801213f0 324
#include "TOBJ.H"
extern unsigned short D_8012F2F8[];
extern short D_1F80019E;
extern int func_800428C0(TObj *, TObj *);

void func_801213F0(TObj *a, TObj *b)
{
    short dz, k, n;
    unsigned short t;
    unsigned short *p;

    if (b->active & 4) return;
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return;
    dz = a->y.p.whole - b->y.p.whole;
    t = dz + (b->box2 + a->box2);
    if ((unsigned short)t > a->box3 + b->box3) return;
    p = D_8012F2F8;
    k = -dz;
    n = k - 0x18;
    if (n < 0) n = 0;
    t = p[n >> 3];
    if ((unsigned short)(a->h->p.whole - (b->h->p.whole + t) + (b->box0 + a->box0)) > b->box1 + a->box1) return;
    if (func_800428C0(a, b) != 0) {
        b->animFrame = a->animFrame & 1;
    }
    D_1F80019E = 0;
}
