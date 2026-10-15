// FUNC 8011e990 1544 X010
// MATCHING 8011e990 1544
#include "spad.h"
/* Matched (opusAD): two `asm("" :: "r"(cx))` use pairs (one at the top after the declarations, one before the
   first k-odd wba/wb8 store) raise cx's ref count so global alloc orders it ahead of dy and the slope: cx s5,
   slope s7 as in the game. */
/* score 28 (opusZ, was 92): case 0 of the second switch(mode) solved: case 0 first, written as a full if/else
   (`if ((short)dx < 0) { if (a->animFrame & 1) return 0; } else { if (!(a->animFrame & 1)) return 0; } break;`), case 1/2
   keeps the goto neg/pos form; the full if/else keeps a label in the case-0 block so jump.c does not swap it with the
   dispatch tree, and the post-reload cross-jump then reduces it to the game`s `bltz neg; j pos`. Also: no a/b pins
   (plain params; pins moved `sb a2,16(sp)`/`move s4,a3`), no ang bump, `(short)(ang - ax)` (no sext), dx/dy unsigned short with (short) casts (abs is `if ((short)dx < 0) s = -dx; else s = dx;`: move s0,s3), `ax = (animFrame & 1) ? 8 : -8` for k (a0), `asm("" : "=r"(b) : "0"(b))` after the d test (b s2 / dx s3) and
   `asm("" :: "r"(dy))` after the wb8 store (ang s4 / dy s6). Left: cx in s7 and s7 in s5 (game: cx s5, s7 s7).
   Previous: */
/* score 92 (opusT, was 232): carried over from the x001 lostHatManTalk match (template x010 collideEnemyWithTiltedBar): box sums
   through ax/ay (`ax = b->box0 + dx; if (b->box1 < (unsigned short)ax)`), the angle base reuses ax with
   `(short)(ang - ax) & 0xfff`, `ax = cx; if (ax < 0) ax = -ax`; second-half sums written into ax / `s = ay - s` in place,
   first-half `ay = ... + 4` and `s = ax - s`; abs through `static __inline__ short sabs(short x) { if (x < 0) return -x;
   return x; }` with `if (s > ax) return 2` (no CSE with the dx*cy mult any more); k test written `a->h + k > b->h + s7`;
   mode switch with case 1/2 first (+ asm("") there) gives the game's beqz/bltz/slti tree; a->wba/a->wb8 stored in both
   branches (cross-jumped) instead of a t temp. All register pins removed. Left: cx and s7 swapped (game cx=s5, s7=s7;
   ours s7/s5; cx bumps/pins/decl order/types do not flip it), abs returns the sra'd x (game moves raw dx: move s0,s3),
   case 0 of the mode switch is placed after the tree's j in the game, k in v1 (game a0).
   Tried (opusW, all still 92 or worse): case 0 placed before case 1/2 in the second mode switch gives the game's
   separate `bltz l48; j l64` case-0 block (no cross-jump) but the dispatch becomes bnez-around (135); 800 asm("")
   placements in that switch: best 92. abs `s = dx; if (s < 0) s = -dx;` gives the game's bgez/move s0,s3/negu but
   CSE shares the test's sra with the dx*cy mult (168, only that mult and dy s5/s6 left); blocking it with an asm copy
   of dx before the mult works but leaves a move and reshuffles s-regs (214). k: pins/ternary/int/sum temps keep v1
   (pin $4: 104). Single-line asm bumps of cx/s7/dx/asm("") at every line (itbrute): none below 92.
   Older (232, opusR, was 236): abs as `s = dx; if (s < 0) s = -dx;` gives the game's move s0,dx / negu s0,dx; the
   game tests dx<<16 directly (no sra, so its sext is not shared with the dx*cy mult) where ours tests s0<<16;
   `if (dx < 0)` makes CSE share the sra with the mult (264), inline short sabs 258. All five pins still help
   (removing any: 242-375). The game compares sext(s) (v1) first: slt v0(ax),v1(s). Same family as x001
   lostHatManTalk (selector/abs/box-sum shapes there).
   Older (236, was 307): in-place abs `s = dx; if (s < 0) s = -dx; if (ax < s) return 2;` with s pinned $16 lets the
   four `return 2` sites take li v0,2 in the delay slot as in the game (290); greedy pins k/$16, q/$21, ax/$4, t/$23 (236, pin
   hacks, likely not the original form). Older note: logic and size match (whole 1544 B incl. csv piece 8011EAE8); remaining diffs are register allocation: game has a=s1, dx=s3, dy=s6, cx=s5, abs value s=s0, fp/s7 for the two slopes, mode kept on the stack (sb a2,0x10(sp)); ours puts a in s0, dx in s1 and s in s7. Sibling wip collidePlayerWithTiltedBar has the same family of diffs. Tried (o27): box sums through ax/ay + in-place abs (313; helped sibling 80121534); priorities from cc1 -dl: game needs s (abs, 8 refs/201 insns) above a (24/293), i.e. s0 for s, which no tried form gives. */
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

int probeTiltedBarContact(TObj *a, TObj *b, unsigned char mode, int ang)
{
    unsigned short dx;
    unsigned short dy;
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
asm("" :: "r"(cx)); asm("" :: "r"(cx));

    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return 0;
asm("" : "=r"(b) : "0"(b));
    dx = XA(a)->we8 - b->h->p.whole;
    ax = b->box0 + dx;
    if (b->box1 < (unsigned short)ax) return 0;
    dy = XA(a)->wea - b->y.p.whole;
    ay = b->box0 + dy;
    if (b->box1 < (unsigned short)ay) return 0;
    switch (mode) {
    case 1:
    case 2:
        q = ((short)dy < 0) ? 0xc00 : 0;
        ax = q;
        if ((short)dx > 0) {
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
        if ((short)dx < 0) s = -dx; else s = dx; if (s > ax) return 2;
        fp = (short)dx * cy / cx;
        break;
    case 1:
        if ((short)dy < 0) s = -dy; else s = dy; if (s > ay) return 2;
        s7 = (short)dy * cx / -cy;
        break;
    case 2:
        if ((short)dx < 0) s = -dx; else s = dx; if (s > ax) return 2;
        fp = 0;
        break;
    case 3:
        if ((short)dy < 0) s = -dy; else s = dy; if (s > ay) return 2;
        s7 = 0;
        break;
    }
    if (!(kind & 1)) {
        ay = XA(a)->wea - (b->y.p.whole - fp) + 4;
        if ((unsigned short)ay >= 9) return 0;
        s = ax - s;
        if ((unsigned short)s >= 0x1d) return 0;
        switch (mode) {
        case 0:
            if ((short)dx < 0) {
                if (a->animFrame & 1) return 0;
            } else {
                if (!(a->animFrame & 1)) return 0;
            }
            break;
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
        if (mode == 0 && (short)dx <= 0) a->wb8 = -cx;
        else a->wb8 = cx;
asm("" :: "r"(dy));
        a->wba = -fp + 0x10;
        a->velY = 0;
        b->velX = dx;
        b->b69 = 1;
        SPAD.s19e = 0;
        (*(TObj **)&SPAD.p3c0) = b;
        b->w7a = mode;
        return 1;
    }
    ax = (a->animFrame & 1) ? 8 : -8;
    if (a->h->p.whole + ax > b->h->p.whole + s7) {
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
asm("" :: "r"(cx)); asm("" :: "r"(cx));
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
    if ((short)dx > 0) dx += 10;
    else if ((short)dx < 0) dx -= 10;
    b->velX = dx;
    b->w7a = mode;
    a->velY = 0;
    a->b9e = 8;
    SPAD.s19e = 0;
    (*(TObj **)&SPAD.p3c0) = b;
    return 1;
}
