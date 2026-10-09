// FUNC 80127074 1544 X001
/* score 496: first full transcription (logic follows the game; register allocation and block layout far off:
   game keeps t spilled at sp+0x10, ang in s4, dx/dy in s3/s6, rcos/rsin products in s5/t0, fy/fx in fp/s7).
   Not tuned yet. */
#include "TOBJ.H"

typedef struct {
    TObj t;
    char pad[0xe8 - 0xc0];
    unsigned short we8, wea;
} PL;

extern short D_1F80019E;
extern TObj *D_1F8003C0;
extern int rcos(int);
extern int rsin(int);

#define ABS(x) ((x) < 0 ? -(x) : (x))

#define o (&p->t)
int func_80127074(PL *p, TObj *e, unsigned char t, int ang)
{
    short dx, dy, c, s, ac, as, a0, m, k;
    short fy, fx;
    int r;

    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) >= 0x5b) return 0;
    dx = p->we8 - e->h->p.whole;
    if (e->box1 < (unsigned short)(e->box0 + dx)) return 0;
    dy = p->wea - e->y.p.whole;
    if (e->box1 < (unsigned short)(e->box0 + dy)) return 0;
    switch (t) {
    case 0:
        break;
    case 1:
    case 2:
        k = (dy < 0) ? 0xc00 : 0;
        a0 = k;
        if (dx > 0) {
            if (k) a0 = k - 0x400;
            else a0 = 0x400;
        }
        if ((unsigned)(((ang - a0) & 0xfff) - 0x500) > 0x900) return 0;
        break;
    default:
        goto skip;
    }
    c = (unsigned)(rcos(ang) * e->box0) >> 12;
    s = (unsigned)(rsin(ang) * e->box0) >> 12;
skip:
    ac = ABS(c);
    as = ABS(s);
    k = ac < as;
    if (ac == 0) k = 3;
    else if (as == 0) k = 2;
    switch (k) {
    case 0:
        m = ABS(dx);
        if (ac < m) return 2;
        fy = dx * s / c;
        break;
    case 1:
        m = ABS(dy);
        if (as < m) return 2;
        fx = dy * c / -s;
        break;
    case 2:
        m = ABS(dx);
        if (ac < m) return 2;
        fy = 0;
        break;
    case 3:
        m = ABS(dy);
        if (as < m) return 2;
        fx = 0;
        break;
    }
    if ((k & 1) == 0) {
        if ((unsigned short)(p->wea - (e->y.p.whole - fy) + 4) >= 9) return 0;
        if ((unsigned short)(ac - m) >= 0x1d) return 0;
        switch (t) {
        case 0:
            if (dx < 0) goto neg;
            goto pos;
        case 1:
        case 2:
            if (c >= 0) goto pos;
        neg:
            if (o->animFrame & 1) return 0;
            break;
        pos:
            if ((o->animFrame & 1) == 0) return 0;
            break;
        }
        o->b9e = 9;
        if (t == 0 && dx <= 0) o->wb8 = -c;
        else o->wb8 = c;
        o->wba = -fy + 0x10;
        o->velY = 0;
        e->velX = dx;
        e->b69 = 1;
        D_1F80019E = 0;
        D_1F8003C0 = e;
        e->w7a = t;
        return 1;
    } else {
        a0 = (o->animFrame & 1) ? 8 : -8;
        if (e->h->p.whole + fx < o->h->p.whole + a0) {
            if (o->b9e == 8) {
                if ((unsigned short)(p->we8 - (e->h->p.whole + fx) + 0x13) >= 0x24) return 0;
                if ((unsigned short)(as - m) < 8) return 0;
            } else {
                if ((unsigned short)(p->we8 - (e->h->p.whole + fx) + 0xb) >= 0x14) return 0;
                if ((unsigned short)(as - m) < 0x10) return 0;
            }
            r = fx + 8;
        } else {
            if (o->b9e == 8) {
                if ((unsigned short)(p->we8 - (e->h->p.whole + fx) + 0x10) >= 0x24) return 0;
            } else {
                if ((unsigned short)(p->we8 - (e->h->p.whole + fx) + 8) >= 0x14) return 0;
            }
            if ((unsigned short)(as - m) < 8) return 0;
            r = fx - 8;
        }
        o->wba = dy;
        o->wb8 = r;
        e->b69 = 1;
        if (dx > 0) dx += 10;
        else if (dx < 0) dx -= 10;
        e->velX = dx;
        e->w7a = t;
        o->velY = 0;
        o->b9e = 8;
        D_1F80019E = 0;
        D_1F8003C0 = e;
        return 1;
    }
}
