// FUNC 80121534 1120 X010
/* score 91: whole function incl. csv piece 80121790. Box sums now go through the abs vars (as = box0+dx,
   ac = box0+dy) and abs is if/else: ang->s2 and box regs match. Left: global-alloc order of c/dy/dx (game
   c=s3 dy=s4 dx=s5; here dy=s3 dx=s4 c=s5), ac/as swapped (game: sum1 and abs(c) share a1, but with ac=sum1
   as gets the higher priority: ac 6 refs/37 insns vs as 6/28), the k copy (game keeps k in a1 apart from q),
   and the even-first branch (li v0,1 hoisted before the y computation). Tried: k via ac/as (sign ext),
   var-name combos for the sums, per-local types (only u16 px helps, semantically wrong), return forms. */
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
    as = e->box0 + dx;
    if (e->box1 < (unsigned short)as)
        return 0;
    dy = o->y.p.whole - e->y.p.whole;
    ac = e->box0 + dy;
    if (e->box1 < (unsigned short)ac)
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
    if (c < 0) ac = -c; else ac = c;
    if (s < 0) as = -s; else as = s;
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
