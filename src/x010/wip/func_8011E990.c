// FUNC 8011e990 1544 X010
/* score 307: logic and size match (whole 1544 B incl. csv piece 8011EAE8); remaining diffs are register allocation: game has a=s1, dx=s3, dy=s6, cx=s5, abs value s=s0, fp/s7 for the two slopes, mode kept on the stack (sb a2,0x10(sp)); ours puts a in s0, dx in s1 and s in s7. Sibling wip func_8011E000 has the same family of diffs. Tried (o27): box sums through ax/ay + in-place abs (313; helped sibling 80121534); priorities from cc1 -dl: game needs s (abs, 8 refs/201 insns) above a (24/293), i.e. s0 for s, which no tried form gives. */
#include "TOBJ.H"

typedef struct { char pad[0xe8]; unsigned short we8, wea; } XA;
#define XA(o) ((XA *)(o))

extern short D_1F80019E;
extern TObj *D_1F8003C0;
extern int rcos(int);
extern int rsin(int);

static __inline__ short sabs(short x)
{
    short r;
    if (x < 0) r = -x; else r = x;
    return r;
}

int func_8011E990(TObj *a, TObj *b, unsigned char mode, int ang)
{
    short dx, dy, cx, cy, ax, ay, s, t;
    short fp, s7, k;
    short kind;
    int q, r;

    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return 0;
    dx = XA(a)->we8 - b->h->p.whole;
    if ((unsigned short)(b->box0 + dx) > b->box1) return 0;
    dy = XA(a)->wea - b->y.p.whole;
    if ((unsigned short)(b->box0 + dy) > b->box1) return 0;
    switch (mode) {
    case 1:
    case 2:
        q = (dy < 0) ? 0xc00 : 0;
        r = q;
        if (dx > 0) {
            if (q == 0) r = 0x400;
            else r = q - 0x400;
        }
        if ((unsigned)(((ang - r) & 0xfff) - 0x500) > 0x900) return 0;
    case 0:
        cx = (unsigned)(rcos(ang) * b->box0) >> 12;
        cy = (unsigned)(rsin(ang) * b->box0) >> 12;
    }
    ax = cx;
    ay = cy;
    if (cx < 0) ax = -cx;
    if (ay < 0) ay = -ay;
    kind = ax < ay;
    if (ax == 0) kind = 3;
    else if (ay == 0) kind = 2;
    switch (kind) {
    case 0:
        if (ax < (s = sabs(dx))) return 2;
        fp = dx * cy / cx;
        break;
    case 1:
        if (ay < (s = sabs(dy))) return 2;
        s7 = dy * cx / -cy;
        break;
    case 2:
        if (ax < (s = sabs(dx))) return 2;
        fp = 0;
        break;
    case 3:
        if (ay < (s = sabs(dy))) return 2;
        s7 = 0;
        break;
    }
    if (!(kind & 1)) {
        if ((unsigned short)(XA(a)->wea - (b->y.p.whole - fp) + 4) >= 9) return 0;
        if ((unsigned short)(ax - s) >= 0x1d) return 0;
        switch (mode) {
        case 0:
            if (dx < 0) goto l48;
            goto l64;
        case 1:
        case 2:
            if (cx >= 0) goto l64;
        l48:
            if (a->animFrame & 1) return 0;
            break;
        l64:
            if (!(a->animFrame & 1)) return 0;
            break;
        }
        a->b9e = 9;
        if (mode == 0 && dx <= 0) a->wb8 = -cx;
        else a->wb8 = cx;
        a->wba = -fp + 0x10;
        a->velY = 0;
        b->velX = dx;
        b->b69 = 1;
        D_1F80019E = 0;
        D_1F8003C0 = b;
        b->w7a = mode;
        return 1;
    }
    if (a->animFrame & 1) k = 8;
    else k = -8;
    if (b->h->p.whole + s7 < a->h->p.whole + k) {
        if (a->b9e == 8) {
            if ((unsigned short)(XA(a)->we8 - (b->h->p.whole + s7) + 0x13) >= 0x24) return 0;
            if ((unsigned short)(ay - s) < 8) return 0;
        } else {
            if ((unsigned short)(XA(a)->we8 - (b->h->p.whole + s7) + 0xb) >= 0x14) return 0;
            if ((unsigned short)(ay - s) < 0x10) return 0;
        }
        t = s7 + 8;
    } else {
        if (a->b9e == 8) {
            if ((unsigned short)(XA(a)->we8 - (b->h->p.whole + s7) + 0x10) >= 0x24) return 0;
        } else {
            if ((unsigned short)(XA(a)->we8 - (b->h->p.whole + s7) + 8) >= 0x14) return 0;
        }
        if ((unsigned short)(ay - s) < 8) return 0;
        t = s7 - 8;
    }
    a->wba = dy;
    a->wb8 = t;
    b->b69 = 1;
    if (dx > 0) dx += 10;
    else if (dx < 0) dx -= 10;
    b->velX = dx;
    b->w7a = mode;
    a->velY = 0;
    a->b9e = 8;
    D_1F80019E = 0;
    D_1F8003C0 = b;
    return 1;
}
