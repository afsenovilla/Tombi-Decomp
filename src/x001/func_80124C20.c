// FUNC 80124c20 532 X001
// MATCHING 80124c20 532
#include "TOBJ.H"

extern unsigned short *D_8013C6A4[];

int func_80124C20(TObj *a, TObj *b)
{
    short dx, off, n, k;
    int d;
    unsigned short t, s;
    unsigned short *p;

    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) > 0x5a) return 0;
    d = (unsigned short)a->y.p.whole - (unsigned short)b->y.p.whole;
    t = d + (b->box2 + a->box2);
    if ((unsigned short)t > a->box3 + b->box3) return 0;
    n = -d;
    k = n - 0x18;
    p = D_8013C6A4[b->subtype];
    if (k < 0) k = 0;
    t = p[k >> 3];
    dx = a->h->p.whole - (b->h->p.whole + t);
    if ((unsigned short)(dx + (b->box0 + a->box0)) > b->box1 + a->box1) return 0;
    if (dx < 0) {
        off = -(b->box0 + a->box0);
    } else {
        off = (b->box1 - b->box0) + (a->box1 - a->box0);
    }
    s = b->box2 + a->box2;
    if (s - n >= 9) {
        a->h->p.whole = b->h->p.whole + t + off;
        if (off < 0) a->ba6 = 2; else a->ba6 = 3;
        return 2;
    }
    if (a->b9c & 1) {
        a->h->p.whole = b->h->p.whole + t + off;
        return 2;
    }
    a->y.p.whole = b->y.p.whole - s;
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
    return 1;
}
