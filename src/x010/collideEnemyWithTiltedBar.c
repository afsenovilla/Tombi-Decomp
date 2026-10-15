// FUNC 80121534 1120 X010
// MATCHING 80121534 1120
/* Covers 80121534 + 80121790 (csv piece). k pinned a1; abs as "ac = c; asm(""); if (c < 0) ac = -c" with |c| sharing
   the box0+dx temp; asm("" : "=r"(ac) : "0"(ac)) in case 0 raises ac's priority; c is an int pinned s3 extended
   through v0-pinned copies; py case extends dy first; b69 = 1 through a v0-pinned temp after an asm barrier. */
#include "TOBJ.H"
extern int FUN_80063548(int);
extern int FUN_8006347c(int);

short collideEnemyWithTiltedBar(TObj *o, TObj *e, unsigned char mode, int ang)
{
    short dx;
    short dy;
    register int c asm("$19");
    short s;
    short ac;
    short as;
    short px;
    short py;
    short sel;
    short t;
    register int k asm("$5");
    int K, KC;
    short r;

    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) >= 0x5b)
        return 0;
    dx = o->h->p.whole - e->h->p.whole;
    ac = e->box0 + dx;
    if (e->box1 < (unsigned short)ac)
        return 0;
    dy = o->y.p.whole - e->y.p.whole;
    as = e->box0 + dy;
    if (e->box1 < (unsigned short)as)
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
        { register int u asm("$2"); u = ang - k; if ((unsigned)((u & 0xfff) - 0x500) > 0x900)
            return 1; }
    case 0:
        c = (unsigned)(FUN_80063548(ang) * e->box0) >> 12;
        s = (unsigned)(FUN_8006347c(ang) * e->box0) >> 12;
        break;
    }
    ac = c; asm(""); if ((short)c < 0) ac = -c;
    as = s; asm(""); if (s < 0) as = -s;
    sel = ac < as;
    if (ac == 0)
        sel = 3;
    else if (as == 0)
        sel = 2;
    switch (sel) {
    case 0:
        if (dx < 0) t = -dx;
        else t = dx;
        asm("" : "=r"(ac) : "0"(ac));
        if (t > ac)
            return 2;
        { register int cp asm("$2"); cp = (short)c; px = dx * s / cp; }
        break;
    case 1:
        if (dy < 0) t = -dy;
        else t = dy;
        if (t > as)
            return 2;
        { int d = dy; register int cp asm("$2"); asm(""); cp = (short)c; py = d * cp / -s; }
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
            asm("");
            { register int one asm("$2") = 1; o->b69 = one; return one; }
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
