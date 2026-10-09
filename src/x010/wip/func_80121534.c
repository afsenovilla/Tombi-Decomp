// FUNC 80121534 1120 X010
/* score 135: whole function incl. csv piece 80121790. Control flow matches except the even-first
   branch (game computes y in v0 and reuses v0=1 for b69 and the return) and the k/q copy
   (game keeps k in a1 apart from q). The rest is global register allocation: game has
   ang=s2 c=s3 dy=s4 dx=s5 px=s6 py=s7, box1 in t0, sel in a2. Tried: types, compare order, if/else forms. */
#include "TOBJ.H"
extern int FUN_80063548(int);
extern int FUN_8006347c(int);

short func_80121534(TObj *o, TObj *e, unsigned char mode, int ang)
{
    short dx;
    short dy;
    short c;
    short s;
    short ac;
    short as;
    short px;
    short py;
    short sel;
    short t;
    int k;
    int K, KC;
    short r;

    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) >= 0x5b)
        return 0;
    dx = o->h->p.whole - e->h->p.whole;
    if (e->box1 < (unsigned short)(e->box0 + dx))
        return 0;
    dy = o->y.p.whole - e->y.p.whole;
    if (e->box1 < (unsigned short)(e->box0 + dy))
        return 0;
    switch (mode) {
    case 1:
    case 2:
        {
            int q = (dy >> 31) & 0xc00;
            k = q;
            if (dx > 0) {
                if (!q) k = 0x400;
                else k = q - 0x400;
            }
        }
        if ((unsigned)(((ang - k) & 0xfff) - 0x500) > 0x900)
            return 1;
    case 0:
        c = (unsigned)(FUN_80063548(ang) * e->box0) >> 12;
        s = (unsigned)(FUN_8006347c(ang) * e->box0) >> 12;
        break;
    }
    ac = c;
    as = s;
    if (c < 0) ac = -c;
    if (s < 0) as = -s;
    sel = ac < as;
    if (ac == 0)
        sel = 3;
    else if (as == 0)
        sel = 2;
    switch (sel) {
    case 0:
        if (dx < 0) t = -dx;
        else t = dx;
        if (t > ac)
            return 2;
        px = dx * s / c;
        break;
    case 1:
        if (dy < 0) t = -dy;
        else t = dy;
        if (t > as)
            return 2;
        py = dy * c / -s;
        break;
    case 2:
        px = 0;
        break;
    case 3:
        py = 0;
        break;
    }
    K = 4;
    KC = 0xc;
    if (!(sel & 1)) {
        if (o->y.p.whole < e->y.p.whole - px) {
            if (e->y.p.whole - (px + K) > o->y.p.whole + o->box3 - o->box2)
                return 3;
            o->y.p.whole = e->y.p.whole - (px + K) - (o->box3 - o->box2);
            o->b69 = 1;
            return 1;
        } else {
            if (e->y.p.whole - (px - K) < o->y.p.whole - o->box2)
                return 3;
            o->y.p.whole = o->box2 + (e->y.p.whole - (px - K));
        }
    } else {
        if (e->h->p.whole + py < o->h->p.whole) {
            if (e->h->p.whole + (py + KC) < o->h->p.whole - o->box0)
                return 3;
            o->h->p.whole = o->box0 + (e->h->p.whole + (py + KC));
            r = 3;
        } else {
            if (e->h->p.whole + (py - KC) > o->h->p.whole + (o->box1 - o->box0))
                return 3;
            o->h->p.whole = e->h->p.whole + (py - KC) - (o->box1 - o->box0);
            r = 2;
        }
        o->b9d = r;
        return r;
    }
}
