// FUNC 801210e4 496 X010
// MATCHING 801210e4 496
#include "TOBJ.H"
extern unsigned short D_8012F2F8[];

void func_801210E4(TObj *a, TObj *b)
{
    short dz, k, n, dx, off;
    unsigned short t;
    unsigned short *p;
    unsigned short w;

    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return;
    dz = a->y.p.whole - b->y.p.whole;
    t = dz + (b->box2 + a->box2);
    if ((unsigned short)t > a->box3 + b->box3) return;
    p = D_8012F2F8;
    k = -dz;
    n = k - 0x18;
    if (n < 0) n = 0;
    t = p[n >> 3];
    dx = a->h->p.whole - (b->h->p.whole + t);
    if ((unsigned short)(dx + (b->box0 + a->box0)) > b->box1 + a->box1) return;
    if (dx < 0) off = -(b->box0 + a->box0);
    else off = (b->box1 - b->box0) + (a->box1 - a->box0);
    w = b->box2 + a->box2;
    if ((unsigned short)w - k >= 9) {
        a->h->p.whole = off + (b->h->p.whole + t);
        if (off < 0) a->ba6 = 2;
        else a->ba6 = 3;
        return;
    }
    if (a->b9c & 1) {
        a->h->p.whole = off + (b->h->p.whole + t);
        return;
    }
    a->y.p.whole = b->y.p.whole - w;
    a->y.p.frac = 0;
    a->b69 = 1;
    b->b69 = 1;
    if (off >= 0) {
        a->bbe = 8;
        a->wb0 = 2;
    } else {
        a->bbe = 9;
        a->wb0 = -2;
    }
}
