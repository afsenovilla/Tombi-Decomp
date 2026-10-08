// FUNC 8003f7cc 768 MAIN0
/* score 34: type search (hi int, t unsigned short) from 84. Left: s-reg numbering (game dir2=s4, lo=s5, hi=s7), k copy (move v1,a0) in the box2 clamp, dir = t copy (game passes a3=t to the first call, no andi). */
#include "TOBJ.H"

short func_8004065C(TObj *o, short x, short y, int dir);

short func_8003F7CC(TObj *o)
{
    short d;
    short dx;
    short dx2;
    short lo;
    int hi;
    short r;
    int h;
    short k;
    int dir;
    int dir2;
    unsigned short t;
    char pad[8];
    if (*(unsigned char *)&o->waa != 0) return 0;
    if (o->active == 5) return 0;
    d = o->h->p.whole - *(unsigned short *)0x1F80016A;
    if (d == 0) {
        if (!(o->animFrame & 1)) {
            dx = 8;
            goto right;
        }
        dx = -8;
        goto left;
    }
    if (d >= 0) {
        dx = 8;
right:
        dx2 = -8;
        t = 0;
        dir2 = 1;
    } else {
        dx = -8;
left:
        dx2 = 8;
        t = 1;
        dir2 = 0;
    }
    h = o->box2;
    k = h;
    lo = k;
    if (h >= 8) lo = 8;
    hi = k;
    if (h >= 14) hi = 14;
    dir = t;
    r = func_8004065C(o, o->h->p.whole + dx, o->y.p.whole - lo, t);
    if (r) {
        if (dir == (o->animFrame & 1)) { if (!dir) o->ba6 = 2; else o->ba6 = 3; }
    } else {
        r = func_8004065C(o, o->h->p.whole + dx, o->y.p.whole + hi, dir);
        if (r) {
            if (dir == (o->animFrame & 1)) { if (!dir) o->ba6 = 2; else o->ba6 = 3; }
        } else {
            r = func_8004065C(o, o->h->p.whole + dx, o->y.p.whole + lo, dir);
            if (r) {
                if (dir == (o->animFrame & 1)) { if (!dir) o->ba6 = 2; else o->ba6 = 3; }
            }
        }
    }
    dir = dir2;
    if (func_8004065C(o, o->h->p.whole + dx2, o->y.p.whole + hi, dir)) {
        if (dir == (o->animFrame & 1)) { if (!dir) o->ba6 = 2; else o->ba6 = 3; }
    } else if (func_8004065C(o, o->h->p.whole + dx2, o->y.p.whole + lo, dir)) {
        if (dir == (o->animFrame & 1)) { if (!dir) o->ba6 = 2; else o->ba6 = 3; }
    }
    return r;
}
