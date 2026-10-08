// FUNC 8003f7cc 768 MAIN0
/* score 123: structure ok; regalloc differs (game passes dir in a3 then copies to s1; h in a0 with copy) */
#include "TOBJ.H"

short func_8004065C(TObj *o, short x, short y, int dir);

short func_8003F7CC(TObj *o)
{
    short d, dx, dx2, lo, hi, r;
    int h;
    int dir, dir2;
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
        dir = 0;
        dir2 = 1;
    } else {
        dx = -8;
left:
        dx2 = 8;
        dir = 1;
        dir2 = 0;
    }
    h = o->box2;
    lo = h;
    if (h >= 8) lo = 8;
    hi = h;
    if (h >= 14) hi = 14;
    r = func_8004065C(o, o->h->p.whole + dx, o->y.p.whole - lo, dir);
    if (r) {
        if (dir == (o->animFrame & 1)) o->ba6 = dir ? 3 : 2;
    } else {
        r = func_8004065C(o, o->h->p.whole + dx, o->y.p.whole + hi, dir);
        if (r) {
            if (dir == (o->animFrame & 1)) o->ba6 = dir ? 3 : 2;
        } else {
            r = func_8004065C(o, o->h->p.whole + dx, o->y.p.whole + lo, dir);
            if (r) {
                if (dir == (o->animFrame & 1)) o->ba6 = dir ? 3 : 2;
            }
        }
    }
    dir = dir2;
    if (func_8004065C(o, o->h->p.whole + dx2, o->y.p.whole + hi, dir)) {
        if (dir == (o->animFrame & 1)) o->ba6 = dir ? 3 : 2;
    } else if (func_8004065C(o, o->h->p.whole + dx2, o->y.p.whole + lo, dir)) {
        if (dir == (o->animFrame & 1)) o->ba6 = dir ? 3 : 2;
    }
    return r;
}
