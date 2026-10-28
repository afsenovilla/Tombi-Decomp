// FUNC 80127074 1544 X001
// MATCHING 80127074 1544
// VERSION SLPS_01144 x01 801259B8 1544
// VERSION SLPS_01144 x10 8011D014 1544
#if defined(VERSION_SLPS_01144)
#include "spad.h"
/* Matched (opusAD): two `asm("" :: "r"(cx))` use pairs (one at the top after the declarations, one before the
   first k-odd wba/wb8 store) raise cx's ref count so global alloc orders it ahead of dy and the slope: cx s5,
   slope s7 as in the game. */
/* score 28 (opusZ, was 95): case 0 of the second switch(t) solved: case 0 first, written as a full if/else
   (`if ((short)dx < 0) { if (animFrame & 1) return 0; } else { if (!(animFrame & 1)) return 0; } break;`), case 1/2
   keeps the goto neg/pos form; the full if/else keeps a label in the case-0 block so jump.c does not swap it with the
   dispatch tree, and the post-reload cross-jump then reduces it to the game`s `bltz neg; j pos`. Also: no p/e pins
   (plain params; pins moved `sb a2,16(sp)`/`move s4,a3`), no ang bump, `(short)(ang - ax)` (no sext), short cx/cy
   (fixes `move a2,t0`), no ax/ay `=r` barriers, `asm("" : "=r"(e) : "0"(e))` after the d test (e s2 / dx s3) and
   `asm("" :: "r"(dy))` after the wb8 store (ang s4 / dy s6). Left: cx in s7 and fx in s5 (game: cx s5, fx s7).
   Previous: */
/* score 95 (opusX, was 223): ported the 801253AC/801278C0 shapes. ax/ay are the box-sum temps (`ax = e->box0 + dx;
   if ((unsigned short)ax > e->box1)`, `ay = ...dy`), the quadrant base (ax = base / 0x400 / base - 0x400 with the
   `base == 0` test first), the abs copies, the +4 sum (ay, k even) and the +0x13/+0xb/+0x10/+8 sums (ax, k odd); the
   +-8 offset is ax too, compared as `o->h->p.whole + ax > e->h->p.whole + fx`; the k odd tail stores wba/wb8 in each
   branch (cross-jumped); asm bumps on ang/cx at the top, `=r` barriers on ax/ay after the abs copies and an
   `asm("" :: "r"(dy))` in case 1/2 fix the s-reg permutation; no pins on ax/ay/k. Left: ang copy `move s4,a3` comes
   before the saves (ang bump; without it s-regs permute, 127), `ang - ax` sign-extends ax (game uses a0 as is; int b
   for the base: 158), `move a2,t0` scheduled before the cx sign test, case 0 of the second switch(t) is
   `bltz neg; j pos` in the game (ours falls into the case 1/2 bgez; tried split if/else bodies 159-177).
   Previous: */
/* score 223 (opusS, was 272): no c/s copies of cx/cy (cx used directly for the sign test and wb8, as the game does
   with s5); abs block in sibling order `ax = cx; ay = cy; if (ax < 0) ax = -ax; if (ay < 0) ay = -ay;` (game copies
   both before the tests); `m = ax - m;` / `m = ay - m;` before the 0x1d/8/0x10 tests (game reuses m's s0);
   greedy.py pins ax $4, ay $6, k $4 (289 -> 223). Left: box temps (game box0+dx sum in a0/a2, box1 in a3), t stored
   to 16(sp) after the d test, quadrant base a0/v1, cx in s6 (game s5) and dx/fy/fx s-reg permutation, case 2/3
   compares, `+4`/`+0x13`/`+0xb` sums go to a2/a0 in the game (a var assignment?), a0 = +-8 sign-extended in the game.
   Tried: box0+dx into ax/ay first (266-270), int/ushort a0 (223-250), itbrute asm bumps (221, fy bump in case 1, not kept).
   Previous: */
/* score 272 (opusN, was 314; size 1544 = game): switch(t) written with case 1/2 first (`goto neg`) and case 0
   `if ((short)dx >= 0) goto pos;` (game tests t with beqz); m as `if ((short)dx < 0) m = -dx; else m = dx;` (m in s0
   directly, no sra temp reused by the mult); `if (m > ax) return 2` operand order. Tried: sabs() inline for m (308,
   result copied v1 -> m), short dx/dy (no change), dropping c/s for cx (297, size 1532: case 2/3 cross-jumped),
   ax asm barrier / c copy orders (no change). Left: the extra c copy in the abs block (game ax = cx in a0, negu a0,a0),
   k/ax/ay regs (a1/a2/a3 vs a3/a0/a2), case 2/3 return-2 jumps to epilogue with li v0,2 in delay, fx/fy s-regs.
   Previous: */
/* score 314 (was 496; 1536 of 1544 B). Rewritten in the style of the sibling x001 collideEnemyWithTiltedBar wip: dx/dy unsigned
   short tested as (short), cx/cy int products, ax/ay/m short with `ax = cx; if (ax < 0) ax = -ax;`, quadrant base copied
   to b, p/e pinned to $17/$18 through register copies of the parameters, r short. Left: t is kept in a stack slot in the
   game (sb $6,16(sp) / lbu) and ours matches that, but abs temps and k/fx/fy regs differ (k $7, fx $23, fy $30,
   cx $21), a0 = +-8 is sign-extended in the game, (ax - m) reuses m's reg, second switch(t) case order. */
#include "TOBJ.H"

typedef struct {
    TObj t;
    char pad[0xe8 - 0xd0];
    unsigned short we8, wea;
} PL;

extern int rcos(int);
extern int rsin(int);

#define o (&p->t)
int seesawPlankProbeFace(PL *p, TObj *e, unsigned char t, int ang)
{
    unsigned short dx, dy;
    short cx;
    short cy;
    short m;
    short k;
    short fy;
    short fx;
    int base, b;
    short ax;
    short ay;
asm("" :: "r"(cx)); asm("" :: "r"(cx));

    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) >= 0x5b) return 0;
asm("" : "=r"(e) : "0"(e));
    dx = p->we8 - e->h->p.whole;
    ax = e->box0 + dx;
    if ((unsigned short)ax > e->box1) return 0;
    dy = p->wea - e->y.p.whole;
    ay = e->box0 + dy;
    if ((unsigned short)ay > e->box1) return 0;
    switch (t) {
    case 1:
    case 2:
        base = (short)dy < 0 ? 0xc00 : 0;
        ax = base;
        if ((short)dx > 0) {
            if (base == 0) ax = 0x400;
            else ax = base - 0x400;
        }
        if ((unsigned)(((short)(ang - ax) & 0xfff) - 0x500) >= 0x901) return 0;
    case 0:
        cx = (unsigned)(rcos(ang) * e->box0) >> 12;
        cy = (unsigned)(rsin(ang) * e->box0) >> 12;
        break;
    }
    ax = cx;
    ay = cy;
    if (ax < 0) ax = -ax;
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
        ay = p->wea - (e->y.p.whole - fy) + 4;
        if ((unsigned short)ay >= 9) return 0;
        m = ax - m;
        if ((unsigned short)m >= 0x1d) return 0;
        switch (t) {
        case 0:
            if ((short)dx < 0) {
                if (o->animFrame & 1) return 0;
            } else {
                if ((o->animFrame & 1) == 0) return 0;
            }
            break;
        case 1:
        case 2:
            if ((short)cx >= 0) goto pos;
        neg:
            if (o->animFrame & 1) return 0;
            break;
        pos:
            if ((o->animFrame & 1) == 0) return 0;
            break;
        }
        o->b9e = 9;
        if (t == 0 && (short)dx <= 0) o->wb8 = -cx;
        else o->wb8 = cx;
asm("" :: "r"(dy));
        o->wba = -fy + 0x10;
        o->velY = 0;
        e->velX = dx;
        e->b69 = 1;
        SPAD.s19e = 0;
        (*(TObj **)&SPAD.p3c0) = e;
        e->w7a = t;
        return 1;
    } else {
        ax = (o->animFrame & 1) ? 8 : -8;
        if (o->h->p.whole + ax > e->h->p.whole + fx) {
            if (o->b9e == 8) {
                ax = p->we8 - (e->h->p.whole + fx) + 0x13;
                if ((unsigned short)ax >= 0x24) return 0;
                m = ay - m;
                if ((unsigned short)m < 8) return 0;
            } else {
                ax = p->we8 - (e->h->p.whole + fx) + 0xb;
                if ((unsigned short)ax >= 0x14) return 0;
                m = ay - m;
                if ((unsigned short)m < 0x10) return 0;
            }
asm("" :: "r"(cx)); asm("" :: "r"(cx));
            o->wba = dy;
            o->wb8 = fx + 8;
        } else {
            if (o->b9e == 8) {
                ax = p->we8 - (e->h->p.whole + fx) + 0x10;
                if ((unsigned short)ax >= 0x24) return 0;
            } else {
                ax = p->we8 - (e->h->p.whole + fx) + 8;
                if ((unsigned short)ax >= 0x14) return 0;
            }
            m = ay - m;
            if ((unsigned short)m < 8) return 0;
            o->wba = dy;
            o->wb8 = fx - 8;
        }
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
#else
#include "spad.h"
/* Matched (opusAD): two `asm("" :: "r"(cx))` use pairs (one at the top after the declarations, one before the
   first k-odd wba/wb8 store) raise cx's ref count so global alloc orders it ahead of dy and the slope: cx s5,
   slope s7 as in the game. */
/* score 28 (opusZ, was 95): case 0 of the second switch(t) solved: case 0 first, written as a full if/else
   (`if ((short)dx < 0) { if (animFrame & 1) return 0; } else { if (!(animFrame & 1)) return 0; } break;`), case 1/2
   keeps the goto neg/pos form; the full if/else keeps a label in the case-0 block so jump.c does not swap it with the
   dispatch tree, and the post-reload cross-jump then reduces it to the game`s `bltz neg; j pos`. Also: no p/e pins
   (plain params; pins moved `sb a2,16(sp)`/`move s4,a3`), no ang bump, `(short)(ang - ax)` (no sext), short cx/cy
   (fixes `move a2,t0`), no ax/ay `=r` barriers, `asm("" : "=r"(e) : "0"(e))` after the d test (e s2 / dx s3) and
   `asm("" :: "r"(dy))` after the wb8 store (ang s4 / dy s6). Left: cx in s7 and fx in s5 (game: cx s5, fx s7).
   Previous: */
/* score 95 (opusX, was 223): ported the 801253AC/801278C0 shapes. ax/ay are the box-sum temps (`ax = e->box0 + dx;
   if ((unsigned short)ax > e->box1)`, `ay = ...dy`), the quadrant base (ax = base / 0x400 / base - 0x400 with the
   `base == 0` test first), the abs copies, the +4 sum (ay, k even) and the +0x13/+0xb/+0x10/+8 sums (ax, k odd); the
   +-8 offset is ax too, compared as `o->h->p.whole + ax > e->h->p.whole + fx`; the k odd tail stores wba/wb8 in each
   branch (cross-jumped); asm bumps on ang/cx at the top, `=r` barriers on ax/ay after the abs copies and an
   `asm("" :: "r"(dy))` in case 1/2 fix the s-reg permutation; no pins on ax/ay/k. Left: ang copy `move s4,a3` comes
   before the saves (ang bump; without it s-regs permute, 127), `ang - ax` sign-extends ax (game uses a0 as is; int b
   for the base: 158), `move a2,t0` scheduled before the cx sign test, case 0 of the second switch(t) is
   `bltz neg; j pos` in the game (ours falls into the case 1/2 bgez; tried split if/else bodies 159-177).
   Previous: */
/* score 223 (opusS, was 272): no c/s copies of cx/cy (cx used directly for the sign test and wb8, as the game does
   with s5); abs block in sibling order `ax = cx; ay = cy; if (ax < 0) ax = -ax; if (ay < 0) ay = -ay;` (game copies
   both before the tests); `m = ax - m;` / `m = ay - m;` before the 0x1d/8/0x10 tests (game reuses m's s0);
   greedy.py pins ax $4, ay $6, k $4 (289 -> 223). Left: box temps (game box0+dx sum in a0/a2, box1 in a3), t stored
   to 16(sp) after the d test, quadrant base a0/v1, cx in s6 (game s5) and dx/fy/fx s-reg permutation, case 2/3
   compares, `+4`/`+0x13`/`+0xb` sums go to a2/a0 in the game (a var assignment?), a0 = +-8 sign-extended in the game.
   Tried: box0+dx into ax/ay first (266-270), int/ushort a0 (223-250), itbrute asm bumps (221, fy bump in case 1, not kept).
   Previous: */
/* score 272 (opusN, was 314; size 1544 = game): switch(t) written with case 1/2 first (`goto neg`) and case 0
   `if ((short)dx >= 0) goto pos;` (game tests t with beqz); m as `if ((short)dx < 0) m = -dx; else m = dx;` (m in s0
   directly, no sra temp reused by the mult); `if (m > ax) return 2` operand order. Tried: sabs() inline for m (308,
   result copied v1 -> m), short dx/dy (no change), dropping c/s for cx (297, size 1532: case 2/3 cross-jumped),
   ax asm barrier / c copy orders (no change). Left: the extra c copy in the abs block (game ax = cx in a0, negu a0,a0),
   k/ax/ay regs (a1/a2/a3 vs a3/a0/a2), case 2/3 return-2 jumps to epilogue with li v0,2 in delay, fx/fy s-regs.
   Previous: */
/* score 314 (was 496; 1536 of 1544 B). Rewritten in the style of the sibling x001 collideEnemyWithTiltedBar wip: dx/dy unsigned
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

#define o (&p->t)
int seesawPlankProbeFace(PL *p, TObj *e, unsigned char t, int ang)
{
    unsigned short dx, dy;
    short cx;
    short cy;
    short m;
    short k;
    short fy;
    short fx;
    int base, b;
    short ax;
    short ay;
asm("" :: "r"(cx)); asm("" :: "r"(cx));

    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) >= 0x5b) return 0;
asm("" : "=r"(e) : "0"(e));
    dx = p->we8 - e->h->p.whole;
    ax = e->box0 + dx;
    if ((unsigned short)ax > e->box1) return 0;
    dy = p->wea - e->y.p.whole;
    ay = e->box0 + dy;
    if ((unsigned short)ay > e->box1) return 0;
    switch (t) {
    case 1:
    case 2:
        base = (short)dy < 0 ? 0xc00 : 0;
        ax = base;
        if ((short)dx > 0) {
            if (base == 0) ax = 0x400;
            else ax = base - 0x400;
        }
        if ((unsigned)(((short)(ang - ax) & 0xfff) - 0x500) >= 0x901) return 0;
    case 0:
        cx = (unsigned)(rcos(ang) * e->box0) >> 12;
        cy = (unsigned)(rsin(ang) * e->box0) >> 12;
        break;
    }
    ax = cx;
    ay = cy;
    if (ax < 0) ax = -ax;
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
        ay = p->wea - (e->y.p.whole - fy) + 4;
        if ((unsigned short)ay >= 9) return 0;
        m = ax - m;
        if ((unsigned short)m >= 0x1d) return 0;
        switch (t) {
        case 0:
            if ((short)dx < 0) {
                if (o->animFrame & 1) return 0;
            } else {
                if ((o->animFrame & 1) == 0) return 0;
            }
            break;
        case 1:
        case 2:
            if ((short)cx >= 0) goto pos;
        neg:
            if (o->animFrame & 1) return 0;
            break;
        pos:
            if ((o->animFrame & 1) == 0) return 0;
            break;
        }
        o->b9e = 9;
        if (t == 0 && (short)dx <= 0) o->wb8 = -cx;
        else o->wb8 = cx;
asm("" :: "r"(dy));
        o->wba = -fy + 0x10;
        o->velY = 0;
        e->velX = dx;
        e->b69 = 1;
        SPAD.s19e = 0;
        (*(TObj **)&SPAD.p3c0) = e;
        e->w7a = t;
        return 1;
    } else {
        ax = (o->animFrame & 1) ? 8 : -8;
        if (o->h->p.whole + ax > e->h->p.whole + fx) {
            if (o->b9e == 8) {
                ax = p->we8 - (e->h->p.whole + fx) + 0x13;
                if ((unsigned short)ax >= 0x24) return 0;
                m = ay - m;
                if ((unsigned short)m < 8) return 0;
            } else {
                ax = p->we8 - (e->h->p.whole + fx) + 0xb;
                if ((unsigned short)ax >= 0x14) return 0;
                m = ay - m;
                if ((unsigned short)m < 0x10) return 0;
            }
asm("" :: "r"(cx)); asm("" :: "r"(cx));
            o->wba = dy;
            o->wb8 = fx + 8;
        } else {
            if (o->b9e == 8) {
                ax = p->we8 - (e->h->p.whole + fx) + 0x10;
                if ((unsigned short)ax >= 0x24) return 0;
            } else {
                ax = p->we8 - (e->h->p.whole + fx) + 8;
                if ((unsigned short)ax >= 0x14) return 0;
            }
            m = ay - m;
            if ((unsigned short)m < 8) return 0;
            o->wba = dy;
            o->wb8 = fx - 8;
        }
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
#endif
