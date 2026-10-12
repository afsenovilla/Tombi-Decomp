// FUNC 8003f7cc 768 MAIN0
// MATCHING 8003f7cc 768
/* Player grab/push probe: checks five points around the player with func_8004065C.
   Notes on the shape (several agents): `b = o->box2` re-read (cse copy of the lh), the direction
   assignments written in full in all four branches (cross-jumped later; the extra refs rank dir2 above
   lo in global-alloc), `short dir` so the arg promotion temp (single-set, sched1-boosted) is the s1 copy. */
#include "TOBJ.H"

short func_8004065C(TObj *o, short x, short y, int dir);

#define HIT(DIR) \
    if (DIR == (o->animFrame & 1)) { \
        if (!DIR) o->ba6 = 2; else o->ba6 = 3; \
    }

short func_8003F7CC(TObj *o)
{
    short d;
    short dx;
    short dx2;
    short lo;
    short hi;
    short r;
    int h;
    short b;
    short dir;
    short dir2;
    int t;

    if (*(unsigned char *)&o->waa != 0) return 0;
    if (o->active == 5) return 0;
    d = o->h->p.whole - *(unsigned short *)0x1F80016A;
    if (d == 0) {
        if (!(o->animFrame & 1)) {
            dx = 8; dx2 = -8; t = 0; dir2 = 1;
        } else {
            dx = -8; dx2 = 8; t = 1; dir2 = 0;
        }
    } else if (d >= 0) {
        dx = 8; dx2 = -8; t = 0; dir2 = 1;
    } else {
        dx = -8; dx2 = 8; t = 1; dir2 = 0;
    }
    h = o->box2;
    b = o->box2;
    lo = b;
    if (h >= 8) lo = 8;
    hi = b;
    if (h >= 14) hi = 14;
    dir = t;
    r = func_8004065C(o, o->h->p.whole + dx, o->y.p.whole - lo, dir);
    if (r) {
        HIT(dir)
    } else {
        r = func_8004065C(o, o->h->p.whole + dx, o->y.p.whole + hi, dir);
        if (r) {
            HIT(dir)
        } else {
            r = func_8004065C(o, o->h->p.whole + dx, o->y.p.whole + lo, dir);
            if (r) {
                HIT(dir)
            }
        }
    }
    dir = dir2;
    if (func_8004065C(o, o->h->p.whole + dx2, o->y.p.whole + hi, dir)) {
        HIT(dir)
    } else if (func_8004065C(o, o->h->p.whole + dx2, o->y.p.whole + lo, dir)) {
        HIT(dir)
    }
    return r;
}
