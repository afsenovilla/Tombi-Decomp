// FUNC 8011e990 1544 X010
#include "spad.h"
/* score 92 (opusT, was 232): carried over from the x001 func_801253AC match (template x010 func_80121534): box sums
   through ax/ay (`ax = b->box0 + dx; if (b->box1 < (unsigned short)ax)`), the angle base reuses ax with
   `(short)(ang - ax) & 0xfff`, `ax = cx; if (ax < 0) ax = -ax`; second-half sums written into ax / `s = ay - s` in place,
   first-half `ay = ... + 4` and `s = ax - s`; abs through `static __inline__ short sabs(short x) { if (x < 0) return -x;
   return x; }` with `if (s > ax) return 2` (no CSE with the dx*cy mult any more); k test written `a->h + k > b->h + s7`;
   mode switch with case 1/2 first (+ asm("") there) gives the game's beqz/bltz/slti tree; a->wba/a->wb8 stored in both
   branches (cross-jumped) instead of a t temp. All register pins removed. Left: cx and s7 swapped (game cx=s5, s7=s7;
   ours s7/s5; cx bumps/pins/decl order/types do not flip it), abs returns the sra'd x (game moves raw dx: move s0,s3),
   case 0 of the mode switch is placed after the tree's j in the game, k in v1 (game a0).
   Older (232, opusR, was 236): abs as `s = dx; if (s < 0) s = -dx;` gives the game's move s0,dx / negu s0,dx; the
   game tests dx<<16 directly (no sra, so its sext is not shared with the dx*cy mult) where ours tests s0<<16;
   `if (dx < 0)` makes CSE share the sra with the mult (264), inline short sabs 258. All five pins still help
   (removing any: 242-375). The game compares sext(s) (v1) first: slt v0(ax),v1(s). Same family as x001
   func_801253AC (selector/abs/box-sum shapes there).
   Older (236, was 307): in-place abs `s = dx; if (s < 0) s = -dx; if (ax < s) return 2;` with s pinned $16 lets the
   four `return 2` sites take li v0,2 in the delay slot as in the game (290); greedy pins k/$16, q/$21, ax/$4, t/$23 (236, pin
   hacks, likely not the original form). Older note: logic and size match (whole 1544 B incl. csv piece 8011EAE8); remaining diffs are register allocation: game has a=s1, dx=s3, dy=s6, cx=s5, abs value s=s0, fp/s7 for the two slopes, mode kept on the stack (sb a2,0x10(sp)); ours puts a in s0, dx in s1 and s in s7. Sibling wip func_8011E000 has the same family of diffs. Tried (o27): box sums through ax/ay + in-place abs (313; helped sibling 80121534); priorities from cc1 -dl: game needs s (abs, 8 refs/201 insns) above a (24/293), i.e. s0 for s, which no tried form gives. */
#include "TOBJ.H"

typedef struct { char pad[0xe8]; unsigned short we8, wea; } XA;
#define XA(o) ((XA *)(o))

extern int rcos(int);
extern int rsin(int);

static __inline__ short sabs(short x)
{
    if (x < 0) return -x;
    return x;
}

int func_8011E990(TObj *a, TObj *b, unsigned char mode, int ang)
{
    short dx;
    short dy;
    short cx;
    short cy;
    short ax;
    short ay;
    short s;

    short fp;
    short s7;
    short k;
    short kind;
    int q;
    int r;

    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return 0;
    dx = XA(a)->we8 - b->h->p.whole;
    ax = b->box0 + dx;
    if (b->box1 < (unsigned short)ax) return 0;
    dy = XA(a)->wea - b->y.p.whole;
    ay = b->box0 + dy;
    if (b->box1 < (unsigned short)ay) return 0;
    switch (mode) {
    case 1:
    case 2:
        q = (dy < 0) ? 0xc00 : 0;
        ax = q;
        if (dx > 0) {
            if (q == 0) ax = 0x400;
            else ax = q - 0x400;
        }
        if ((unsigned)(((short)(ang - ax) & 0xfff) - 0x500) > 0x900) return 0;
    case 0:
        cx = (unsigned)(rcos(ang) * b->box0) >> 12;
        cy = (unsigned)(rsin(ang) * b->box0) >> 12;
    }
    ax = cx;
    ay = cy;
    if (ax < 0) ax = -ax;
    if (ay < 0) ay = -ay;
    kind = ax < ay;
    if (ax == 0) kind = 3;
    else if (ay == 0) kind = 2;
    switch (kind) {
    case 0:
        s = sabs(dx); if (s > ax) return 2;
        fp = dx * cy / cx;
        break;
    case 1:
        s = sabs(dy); if (s > ay) return 2;
        s7 = dy * cx / -cy;
        break;
    case 2:
        s = sabs(dx); if (s > ax) return 2;
        fp = 0;
        break;
    case 3:
        s = sabs(dy); if (s > ay) return 2;
        s7 = 0;
        break;
    }
    if (!(kind & 1)) {
        ay = XA(a)->wea - (b->y.p.whole - fp) + 4;
        if ((unsigned short)ay >= 9) return 0;
        s = ax - s;
        if ((unsigned short)s >= 0x1d) return 0;
        switch (mode) {
        case 1:
        case 2:
asm("");
            if (cx >= 0) goto l64;
            goto l48;
        case 0:
            if (dx < 0) goto l48;
            goto l64;
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
        SPAD.s19e = 0;
        (*(TObj **)&SPAD.p3c0) = b;
        b->w7a = mode;
        return 1;
    }
    if (a->animFrame & 1) k = 8;
    else k = -8;
    if (a->h->p.whole + k > b->h->p.whole + s7) {
        if (a->b9e == 8) {
            ax = XA(a)->we8 - (b->h->p.whole + s7) + 0x13;
            if ((unsigned short)ax >= 0x24) return 0;
            s = ay - s;
            if ((unsigned short)s < 8) return 0;
        } else {
            ax = XA(a)->we8 - (b->h->p.whole + s7) + 0xb;
            if ((unsigned short)ax >= 0x14) return 0;
            s = ay - s;
            if ((unsigned short)s < 0x10) return 0;
        }
        a->wba = dy;
        a->wb8 = s7 + 8;
    } else {
        if (a->b9e == 8) {
            ax = XA(a)->we8 - (b->h->p.whole + s7) + 0x10;
            if ((unsigned short)ax >= 0x24) return 0;
        } else {
            ax = XA(a)->we8 - (b->h->p.whole + s7) + 8;
            if ((unsigned short)ax >= 0x14) return 0;
        }
        s = ay - s;
            if ((unsigned short)s < 8) return 0;
        a->wba = dy;
        a->wb8 = s7 - 8;
    }
    b->b69 = 1;
    if (dx > 0) dx += 10;
    else if (dx < 0) dx -= 10;
    b->velX = dx;
    b->w7a = mode;
    a->velY = 0;
    a->b9e = 8;
    SPAD.s19e = 0;
    (*(TObj **)&SPAD.p3c0) = b;
    return 1;
}
