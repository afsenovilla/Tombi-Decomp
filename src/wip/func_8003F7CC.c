// FUNC 8003f7cc 768 MAIN0
/* score 84 (dir set as t in branches, dir = t before first call): short k = h copy (game: lh $4 then move $3,$4) fixed the box2 clamp. Still: dir lives in a3 then
   copied to s1 (try a separate branch var passed to the first call), lo/hi/dir2 register numbers, and the
   layout of the 2nd/3rd probe blocks. */
#include "TOBJ.H"

short func_8004065C(TObj *o, short x, short y, int dir);

short func_8003F7CC(TObj *o)
{
    short d, dx, dx2, lo, hi, r;
    int h;
    short k;
    int dir, dir2, t;
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
