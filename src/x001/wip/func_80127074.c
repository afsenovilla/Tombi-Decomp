// FUNC 80127074 1544 X001
#include "spad.h"
/* score 272 (opusN, was 314; size 1544 = game): switch(t) written with case 1/2 first (`goto neg`) and case 0
   `if ((short)dx >= 0) goto pos;` (game tests t with beqz); m as `if ((short)dx < 0) m = -dx; else m = dx;` (m in s0
   directly, no sra temp reused by the mult); `if (m > ax) return 2` operand order. Tried: sabs() inline for m (308,
   result copied v1 -> m), short dx/dy (no change), dropping c/s for cx (297, size 1532: case 2/3 cross-jumped),
   ax asm barrier / c copy orders (no change). Left: the extra c copy in the abs block (game ax = cx in a0, negu a0,a0),
   k/ax/ay regs (a1/a2/a3 vs a3/a0/a2), case 2/3 return-2 jumps to epilogue with li v0,2 in delay, fx/fy s-regs.
   Previous: */
/* score 314 (was 496; 1536 of 1544 B). Rewritten in the style of the sibling x001 func_801278C0 wip: dx/dy unsigned
   short tested as (short), cx/cy int products, ax/ay/m short with `ax = cx; if (ax < 0) ax = -ax;`, quadrant base copied
   to b, p/e pinned to $17/$18 through register copies of the parameters, r short. Left: t is kept in a stack slot in the
   game (sb $6,16(sp) / lbu) and ours matches that, but abs temps and k/fx/fy regs differ (k $7, fx $23, fy $30,
   cx $21), a0 = +-8 is sign-extended in the game, (ax - m) reuses m's reg, second switch(t) case order. */
#include "TOBJ.H"

typedef struct {
    TObj t;
    char pad[0xe8 - 0xc0];
    unsigned short we8, wea;
} PL;

extern int rcos(int);
extern int rsin(int);

static __inline__ short sabs(short x)
{
    short r;
    if (x < 0) r = -x; else r = x;
    return r;
}

#define o (&p->t)
int func_80127074(PL *p_, TObj *e_, unsigned char t, int ang)
{
    register PL *p asm("$17") = p_;
    register TObj *e asm("$18") = e_;
    unsigned short dx, dy;
    int cx, cy;
    short ax, ay, m, k, a0, fy, fx;
    short c, s;
    int base, b; short r;

    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) >= 0x5b) return 0;
    dx = p->we8 - e->h->p.whole;
    if ((unsigned short)(e->box0 + dx) > e->box1) return 0;
    dy = p->wea - e->y.p.whole;
    if ((unsigned short)(e->box0 + dy) > e->box1) return 0;
    switch (t) {
    case 1:
    case 2:
        base = (short)dy < 0 ? 0xc00 : 0;
        b = base;
        if ((short)dx > 0) {
            if (base) b = base - 0x400;
            else b = 0x400;
        }
        if ((unsigned)(((ang - b) & 0xfff) - 0x500) >= 0x901) return 0;
    case 0:
        cx = (unsigned)(rcos(ang) * e->box0) >> 12;
        cy = (unsigned)(rsin(ang) * e->box0) >> 12;
        break;
    }
    c = cx; s = cy;
    ax = cx;
    if (ax < 0) ax = -ax;
    ay = cy;
    if (ay < 0) ay = -ay;
    k = ax < ay;
    if (ax == 0) k = 3;
    else if (ay == 0) k = 2;
    switch (k) {
    case 0:
        if ((short)dx < 0) m = -dx; else m = dx;
        if (m > ax) return 2;
        fy = (short)dx * (short)cy / (short)cx;
        break;
    case 1:
        if ((short)dy < 0) m = -dy; else m = dy;
        if (m > ay) return 2;
        fx = (short)dy * (short)cx / -(short)cy;
        break;
    case 2:
        if ((short)dx < 0) m = -dx; else m = dx;
        if (m > ax) return 2;
        fy = 0;
        break;
    case 3:
        if ((short)dy < 0) m = -dy; else m = dy;
        if (m > ay) return 2;
        fx = 0;
        break;
    }
    if ((k & 1) == 0) {
        if ((unsigned short)(p->wea - (e->y.p.whole - fy) + 4) >= 9) return 0;
        if ((unsigned short)(ax - m) >= 0x1d) return 0;
        switch (t) {
        case 1:
        case 2:
            if (c >= 0) goto pos;
            goto neg;
        case 0:
            if ((short)dx >= 0) goto pos;
        neg:
            if (o->animFrame & 1) return 0;
            break;
        pos:
            if ((o->animFrame & 1) == 0) return 0;
            break;
        }
        o->b9e = 9;
        if (t == 0 && (short)dx <= 0) o->wb8 = -c;
        else o->wb8 = c;
        o->wba = -fy + 0x10;
        o->velY = 0;
        e->velX = dx;
        e->b69 = 1;
        SPAD.s19e = 0;
        (*(TObj **)&SPAD.p3c0) = e;
        e->w7a = t;
        return 1;
    } else {
        a0 = (o->animFrame & 1) ? 8 : -8;
        if (e->h->p.whole + fx < o->h->p.whole + a0) {
            if (o->b9e == 8) {
                if ((unsigned short)(p->we8 - (e->h->p.whole + fx) + 0x13) >= 0x24) return 0;
                if ((unsigned short)(ay - m) < 8) return 0;
            } else {
                if ((unsigned short)(p->we8 - (e->h->p.whole + fx) + 0xb) >= 0x14) return 0;
                if ((unsigned short)(ay - m) < 0x10) return 0;
            }
            r = fx + 8;
        } else {
            if (o->b9e == 8) {
                if ((unsigned short)(p->we8 - (e->h->p.whole + fx) + 0x10) >= 0x24) return 0;
            } else {
                if ((unsigned short)(p->we8 - (e->h->p.whole + fx) + 8) >= 0x14) return 0;
            }
            if ((unsigned short)(ay - m) < 8) return 0;
            r = fx - 8;
        }
        o->wba = dy;
        o->wb8 = r;
        e->b69 = 1;
        if ((short)dx > 0) dx += 10;
        else if ((short)dx < 0) dx -= 10;
        e->velX = dx;
        e->w7a = t;
        o->velY = 0;
        o->b9e = 8;
        SPAD.s19e = 0;
        (*(TObj **)&SPAD.p3c0) = e;
        return 1;
    }
}
