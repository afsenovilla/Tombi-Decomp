// FUNC 80121534 1120 X010
/* score 275: first draft (whole function incl. csv piece 80121790); in progress */
#include "TOBJ.H"
extern int FUN_80063548(int);
extern int FUN_8006347c(int);

short func_80121534(TObj *o, TObj *e, unsigned char mode, int ang)
{
    short dx, dy;
    short c, s;
    short ac, as;
    short px, py;
    short sel;
    int k;

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
        k = (dy >> 31) & 0xc00;
        if (dx > 0) {
            if (k) k -= 0x400;
            else k = 0x400;
        }
        if ((unsigned)(((ang - k) & 0xfff) - 0x500) > 0x900)
            return 1;
    case 0:
        c = (unsigned)(FUN_80063548(ang) * e->box0) >> 12;
        s = (unsigned)(FUN_8006347c(ang) * e->box0) >> 12;
        break;
    }
    ac = c;
    if (c < 0) ac = -c;
    as = s;
    if (s < 0) as = -s;
    if (ac == 0)
        sel = 3;
    else if (as == 0)
        sel = 2;
    else
        sel = ac < as;
    switch (sel) {
    case 0:
        if (ac < (dx < 0 ? -dx : dx))
            return 2;
        px = dx * s / c;
        break;
    case 1:
        if (as < (dy < 0 ? -dy : dy))
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
    if (!(sel & 1)) {
        if (o->y.p.whole < e->y.p.whole - px) {
            if (o->y.p.whole + o->box3 - o->box2 < e->y.p.whole - (px + 4))
                return 3;
            o->y.p.whole = e->y.p.whole - (px + 4) - (o->box3 - o->box2);
            o->b69 = 1;
            return 1;
        } else {
            if (e->y.p.whole - (px - 4) < o->y.p.whole - o->box2)
                return 3;
            o->y.p.whole = o->box2 + (e->y.p.whole - (px - 4));
        }
    } else {
        if (e->h->p.whole + py < o->h->p.whole) {
            if (e->h->p.whole + (py + 0xc) < o->h->p.whole - o->box0)
                return 3;
            o->h->p.whole = o->box0 + (e->h->p.whole + (py + 0xc));
            o->b9d = 3;
            return 3;
        } else {
            if (o->h->p.whole + (o->box1 - o->box0) < e->h->p.whole + (py - 0xc))
                return 3;
            o->h->p.whole = e->h->p.whole + (py - 0xc) - (o->box1 - o->box0);
            o->b9d = 2;
            return 2;
        }
    }
}
