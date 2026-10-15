// FUNC 801244a0 1800 X001
// MATCHING 801244a0 1800
/* Matched (opusAC): in the else arm of the a->wb0 block the wb0 value is a separate var u, and the SPAD.b1d2 w temp v
   is kept live across that arm with `asm("" : "=r"(v));` after the shift and `asm("" :: "r"(v));` after `u = -u` (no code;
   it raises v's global-alloc priority so v gets a1 in the b1d2 block while u stays v1). Same source as x010/func_8011E000.c. */
#include "spad.h"
/* opusU (still 14): split wb0 var u (fn-scope) + w=v gives 26 (v then gets v0). Game's w must be live across the
   wb0 `else` arm (conflict with u=v1 and v0): fake `v = cx;` before `if (u > 8)` in that arm plus `asm("" :: "r"(v));`
   after it gives 4 (only the extra `move a1,s6` copy left), so the real w is some var live there. Using v for the
   arm-2 value (`u = -v`) gives 8 (arm-2 reg a1, game v1). Tried (no gain): w = every existing var with u for wb0,
   v types/decl order, by/sabs/d temps as v, dead `v = X` + end-of-function `ay = v`, wb0 arms with u/v/x combos.
   score 14 (opusQ, was 32): SPAD.b1d2 block fixed: itbrute put `asm("" : "=r"(t) : "0"(t));` right after each
   `t = b->h +/- cx;` (d=v1 now, 26), and the w temp is the function var v (`v = t - a->h + 2; if ((unsigned short)v >= 8)`),
   which makes it a global pseudo (no tie to the andi), giving the game's w=a1, t=a2, h=a3 (14).
   Left: v is now a1 in the a->wb0 block too (game v1 there, so the game's w is NOT v but some other global var that
   conflicts with v0). Tried (no gain): w = kind/ax/ay/s7/q/r/c1/dy/cy/s2/dx/cx (26-110), separate var u for the wb0
   block with fn/block-scope v (26: tie returns), fn/blk-scope w with dead end uses, w -> u copy before the compare,
   int/unsigned v (134), itbrute bumps of v/t/w/h/d around both blocks. */
/* score 32 (n80): two empty clobbers fixed the r and sabs regs: `asm("" ::: "$5", "$6");` just before the
   `(ang - r)` range test puts r in a3 (game), and `asm("" ::: "$3");` after the if/else in sabs() puts its result in v0.
   Left: the SPAD.b1d2 block only (game d=v1, t=a2, w=a1 untied, a->h value=a3; ours d=a2, t=v1, w tied v0, a->h=a1).
   n80 tried (no gain): clobbers ($2..$7 subsets, with/without "memory") before the arms, after t=, after w= (they
   reorder/split the block); `register short t asm("$6")` gives d=v1 but adds a `move a2,v0` copy (73); w/d pins ignored
   or worse; arm rewrites (h temp, h pointer, no w, int w, decl order, int d): all 32 or worse; existing function vars
   reused as w or t (80+); `// AS gnu` / `// CC gcc-2.8.1` (135+). */
/* score 52 (n78): ported x010's `{ int by = b->y.p.whole; ... }` compare temp (fixes the +0x4e8 lh swap).
   n78 tried (no gain): r as ax (game ax=a3 too, but short ax adds sll/sra before `ang - r`; int ax 229), r = int
   box-check temp, sabs forms (game needs jump.c NOT to turn the if/else into `r = x; if (x<0) r = -x;`, so the
   else-arm move lands in the bgez slot and r shares v0 with the test; ~x+1 keeps a nor), sabs result into
   existing vars, function-scope t/w/a->h temps in the SPAD.b1d2 block (size grows).
   Older: score 68 (n73): ang/cx s5/s6 fixed with a weighted ref (debt): `do { c1 = rcos(ang); } while (0);` gives ang 9 refs
   (3*9/178=.152, between dx .159 and cx .148; game order dy>dx>ang>cx). Still left: r in a1 (game a3; r's hard conflicts are
   only v0/sp, nobody prefers a1/a2 - mode's a2 copy dies at the switch; int/short mode, rcos() decl, mode copy: worse),
   sabs result v0/v1 (8 inline/compare forms: no change), +0x4e8 lh a0/v1 swap, SPAD.b1d2 block regs.
   Previous: score 98 (size now 1800 = game): same source as x010/func_8011E000.c (which has the D_8009D2C3 & 0x40 tests).
   Fixed this round: the dead `ay = q;` at the end keeps q alive past the r = q copy (cse rewrites to the reg with the
   later last use, n48), which restores the game's `move a3,v1` copy; in the SPAD.b1d2 block, block-local
   `short t; unsigned short w;` per branch (w = t - a->h + 2; if ((unsigned short)v >= 8)) removes the extra copy of the sum
   (function-scope t gives temp+copy, cse substitutes the temp in the compare).
   Left (register choices only): r gets a1 (game a3: a1/a2 skipped, cause unknown); ang/cx s5/s6 swapped (ours
   cx 10 refs/201 insns beats ang 8 refs/178; game allocates ang first - any dead `v = ax` use flips it but then
   cx uses are rewritten to ax); sabs result v0 vs v1 (game `move v0,s4; negu v0,s4; sll v1,v0`); +0x4e8 block
   lh a0/v1 swap (a->y vs b->y temps); SPAD.b1d2 block: game d=v1, t=a2, w=a1 (untied from the subu temp,
   i.e. non-local), a->h temp=a3; ours d=a2, t=v1, w tied v0, a->h=a1 (w at function scope brings the copy back).
   Tried: int/short/unsigned types of q, r, kind, v, sabs ret/param (no effect), w/t scopes and types (24 combos),
   an `ang` copy variable, ~ang+1 vs -ang (worse), dead uses of ang/cx/kind. */
#include "TOBJ.H"

extern int rcos(int);
extern int rsin(int);

static __inline__ short sabs(short x)
{
    short r;
    if (x < 0) r = -x; else r = x;
    asm("" ::: "$3");
    return r;
}

int collidePlayerWithTiltedBar(TObj *a, TObj *b, unsigned char mode, int ang)
{
    short dx, dy, cx, cy, ax, ay, s2, s7;
    short kind;
    short v;
    short u;
    int q, r, c1;

    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x2d) >= 0x5b) return 0;
    dx = a->h->p.whole - b->h->p.whole;
    ax = b->box0 + dx;
    if ((unsigned short)ax > b->box1) return 0;
    dy = a->y.p.whole - b->y.p.whole;
    ay = b->box0 + dy;
    if ((unsigned short)ay > b->box1) return 0;
    switch (mode) {
    case 1:
    case 2:
        q = (dy < 0) ? 0xc00 : 0;
        r = q;
        if (dx > 0) {
            if (q == 0) r = 0x400;
            else r = q - 0x400;
        }
        asm("" ::: "$5", "$6");
        if ((unsigned)(((ang - r) & 0xfff) - 0x500) > 0x900) return 1;
    case 0:
        do { c1 = rcos(ang); } while (0);
        cx = (unsigned)(c1 * b->box0) >> 12;
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
        if (ax < sabs(dx)) return 2;
        s2 = dx * cy / cx;
        break;
    case 1:
        if (ay < sabs(dy)) return 2;
        s7 = dy * cx / -cy;
        break;
    case 2:
        s2 = 0;
        break;
    case 3:
        s7 = 0;
        break;
    }
    if (!(kind & 1)) {
        if (a->y.p.whole < b->y.p.whole - s2) {
            if ((-(s2 + 4) + b->y.p.whole) > a->y.p.whole + a->box2) return 3;
            if (SPAD.b1d2 != 0) {
                if (dx > 0) a->h->p.whole = b->h->p.whole + cx;
                else a->h->p.whole = b->h->p.whole - cx;
                return 3;
            }
            a->y.p.whole = (-(s2 + 4) + b->y.p.whole) - a->box2;
            a->y.p.frac = 0;
            a->velY = 0;
            a->b69 = 1;
            if (a->b9e != 0) return 3;
            b->velX = dx;
            b->b69 = 1;
            if ((unsigned)(ang - 0x401) < 0x400 || ang > 0xc00) {
                u = (unsigned)(~ang + 1 & 0x3ff) >> 6;
                if (u > 8) u = 8;
            } else {
                u = (unsigned)(ang & 0x3ff) >> 6;
                asm("" : "=r"(v)); /* the game keeps a w temp live across this arm */
                if (u > 8) u = 8;
                u = -u;
                asm("" :: "r"(v));
            }
            a->wb0 = u;
        } else {
            if (a->b04 == 1 && a->step < 2 && sabs(dy) < 8) {
                if ((-(s2 + -4) + b->y.p.whole) < a->y.p.whole - (a->box3 - a->box2)) return 3;
                a->y.p.whole = (-(s2 + -4) + b->y.p.whole) + (a->box3 - a->box2);
                if (dx < 0) a->h->p.whole -= s2;
                else a->h->p.whole += s2;
                return 3;
            }
            { int by = b->y.p.whole; if ((-(s2 + -4) + by) < a->y.p.whole - (a->box3 - a->box2)) return 3; }
            if (SPAD.b1d2 != 0) {
                short d = a->y.p.whole - ((-(s2 + -4) + b->y.p.whole) + (a->box3 - a->box2));
                if (dx > 0) {
                    short t;
                    t = b->h->p.whole + cx;
asm("" : "=r"(t) : "0"(t));
                    v = t - a->h->p.whole + 2;
                    if ((unsigned short)v >= 8) a->h->p.whole += d;
                    else a->h->p.whole = t;
                } else {
                    short t;
                    t = b->h->p.whole - cx;
asm("" : "=r"(t) : "0"(t));
                    v = a->h->p.whole - t + 2;
                    if ((unsigned short)v >= 8) a->h->p.whole -= d;
                    else a->h->p.whole = t;
                }
                return 3;
            }
            a->y.p.whole = (-(s2 + -4) + b->y.p.whole) + (a->box3 - a->box2);
            if (a->velY < 0) a->velY = 0;
        }
    } else {
        s2 = 0x10;
        if (a->b69 != 0 && sabs(dx) < 10) s2 = 4;
        if (b->h->p.whole + s7 < a->h->p.whole) {
            if (b->h->p.whole + s2 + s7 < a->h->p.whole - (a->box1 - a->box0)) return 3;
            a->h->p.whole = s7 + (b->h->p.whole + s2) + (a->box1 - a->box0);
            if (a->velY > 0x300) a->velY = 0x300;
        } else {
            if (b->h->p.whole - s2 + s7 > a->h->p.whole + a->box0) return 3;
            a->h->p.whole = s7 + (b->h->p.whole - s2) - a->box0;
            if (a->velY > 0x300) a->velY = 0x300;
        }
    }
    ay = q;
}
