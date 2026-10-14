// FUNC 80125818 3148 X004
/* opusAA: score 2 = only `sh s0,0xbc` (case 3 wbc = 1) is scheduled before the mult instead of after it
   (tried 3-statement permutations with an m temp (short/int, % before/after), U16, += 1, barriers: all 2-4).
   `asm("" ::: "memory")` barriers after the SETANIM in case 0 (itbrute) and case 4 put the lbu substep first.
   Before: score 11, exactly 787 instrs, fake nop removed. near() is now a statement-expression macro NEAR(o)
   with `short s` and the result `r_ = s == 1 && dy < 0xf8` (gives the game's a0 result, a1 dy and `move v1,s0`).
   case 5: block-local p pinned $3, wba as `register int w asm("$4")` + asm bump, `pv = w + (pv - q)`.
   case 8: `({ register int x asm("$3") = t2 + 4; register unsigned r asm("$2") = (unsigned short)x; r < 8; })`.
   case 10: t3 pinned $3 (`t3 = PLAYER.y; t3 -= U16(o->y)` + bump), o->h via `register Fix16 *hp asm("$4")`,
   and the dy test as an unsigned v0-pinned temp. Left (11): `lbu substep` scheduled after the li in three
   setanim+substep++ tails (cases 0/4), and `sh s0,0xbc` (wbc = 1) in the mult shadow in case 3 (statement
   reorders tried: 11-35). */
#include "spad.h"
#include "player.h"
/* score 159 (opusS, was 204; 3152 of 3148 B, one fake nop): substep AI with 11 cases. opusS applied the three
   deferred size fixes below plus: near() ends in `return chk(s, dy);` with static inline
   chk(int a, unsigned short dy) { if (a != 1) return 0; return dy < 0xf8; } (no v0 copy of the result); case 9 tests
   `d = e + 8` where e is a block-local `wc4/wc8 - U16(o->h)` (game's lhu, a3 temp) and `if (a.x > b.x)` (game loads a.x
   first); case 10 h test written `-o->h->p.whole + PLAYER.h + 0x80`. That is exactly 787 instrs (41 differing lines,
   but scores 328 from the shifted j targets: we lack the game's two `move v1,s0` s copies in near() and have one extra
   nop in case 5 and one extra insn in case 10). The `asm("");` in case 4 (found by itbrute) only adds a nop that
   realigns the code around near() so the score drops to 159; remove it once the s copy is found.
   Left: near(): game result in a0, dy+0x30 in a1 and a copy of s into v1 before the `li v0,1; bne` (tried result
   var, nested inlines returning/modifying the param, NEAR(r) macro: no copy); case 10: game keeps dy in v1
   (subu in place) and loads o->h (a0) before the PLAYER.h load; case 5 p/q regs; case 8 u temp in v1; lbu substep
   scheduled after the li in the setanim+substep++ tails; sh s0,0xbc in the mult shadow in case 3.
   Earlier: o35: case 4's k test is `switch (k)`; opusG: case 5 selects p/q per branch, case 8 as -d + U16(whole).
   The three size fixes (all applied here): case 4 `if (k != 1) { if (k >= 2) break; if (k != 0) break; ... } else`;
   case 5 `t = U16(o->wba) - q + p`; case 10 dy computed before the h test with asm("" : "=r"(t) : "0"(t)). */
#include "TOBJ.H"
typedef struct { short x, y; } P2;
typedef struct {
    TObj o;
    unsigned short wc0, wc2, wc4, wc6, wc8, wca, wcc;
} TX;
#define X(o) ((TX *)(o))
#define U16(f) (*(unsigned short *)&(f))
extern void *D_801345B8, *D_8013460C, *D_80134610, *D_801345CC, *D_801345D0;
extern char D_80077CDC[], D_80077D0C[], D_80077D84[];
extern short D_80131148[];
extern unsigned short D_80131128[];
extern int Rand(void);
extern void SfxPlay(int);
extern void AnimLoadDuration(TObj *);
extern void applyAnimVelocityX(TObj *, int);
extern void applyAnimVelocityY(TObj *, int);
extern int FUN_8001fdac(int, int);
extern int angleBetweenPoints(P2, P2);
extern short FUN_800411cc(TObj *, short, short);
extern short FUN_80041ebc(TObj *, short, short);

#define TICK if (--U16(o->wbc) == 0) { o->wbc = 0x28; if (X(o)->wca) SfxPlay(0x12); }
#define SETANIM(a) do { o->anim = (a); AnimLoadDuration(o); } while (0);

static __inline__ int chk(int a, unsigned short dy) { return a == 1 && dy < 0xf8; }
#define NEAR(o) ({ int r_; short s; unsigned short dy; \
    if (o->d->p.whole != (*(Fix16 **)&PLAYER.obj.d)->p.whole) r_ = 0; else { \
    s = 0; \
    if (((unsigned short)((*(Fix16 **)&PLAYER.obj.h)->p.whole - o->h->p.whole + 0x40) < 0x80 && (Rand() & 0xf) < 0xc) || \
        ((unsigned short)((*(Fix16 **)&PLAYER.obj.h)->p.whole - o->h->p.whole + 0x80) < 0x100 && (Rand() & 0xf) < 6)) \
        s = 1; \
    dy = (*(unsigned short*)&PLAYER.obj.y.p.whole) - o->y.p.whole + 0x30; \
    r_ = s == 1 && dy < 0xf8; } r_; })

void func_80125818(TObj *o)
{
    P2 a, b;
    short d;
    int t;
    int t2;
    unsigned short p;
    int q;
    unsigned short u;
    short k;

    switch (o->substep) {
    case 0:
        if (U16(o->wb4)) {
            o->w22 = 0xf0;
            o->wbc = 1;
            o->wac = 1;
            SETANIM(D_801345B8)
            o->substep = 4;
        } else {
            o->w22 = 0x30;
            o->wb4 = 1;
            o->wbc = 1;
            o->wac = 0x16;
            SETANIM(D_8013460C)
            asm("" ::: "memory");
            o->movetab = D_80077CDC;
            o->substep++;
        }
        break;
    case 1:
        if (--o->w22 == 0) {
            o->wbc = 1;
            o->velH = 0;
            o->velV = 0;
            o->w22 = 0;
            o->wac = 0x17;
            SETANIM(D_80134610)
            o->substep++;
        }
        TICK
        break;
    case 2:
        {
            int v = o->velV;
            o->y.raw += v << 8;
            o->velV -= 0x10;
            o->w22++;
        }
        if (o->velV < -0x1ff || FUN_800411cc(o, o->h->p.whole, o->y.p.whole - 0x18)) {
            o->wbc = 1;
            o->substep++;
        }
        TICK
        break;
    case 3:
        {
            int v = o->velV;
            o->y.raw += v << 8;
            o->velV += 0x10;
            o->w22++;
        }
        if (o->velV >= 0 || FUN_800411cc(o, o->h->p.whole, o->y.p.whole - 0x18)) {
            o->wac = 1;
            SETANIM(D_801345B8)
            o->wbc = 1;
            o->substep++;
            o->w22 = 0xf0 - o->w22 % 40;
        }
        TICK
        break;
    case 4:
        if (--U16(o->wbc) == 0) {
            o->wbc = 0x38;
            if (X(o)->wca) SfxPlay(0x12);
        }
        if (--o->w22 == 0) {
            o->timer = 0xf0;
            o->wac = 1;
            SETANIM(D_801345B8)
            o->substep = 9;
        }
        if (o->w22 != 0xb4 && o->w22 != 0x3c) break;
        if (NEAR(o)) {
            o->step = 1;
            o->state = 0;
            o->substep = 0;
            break;
        }
        o->d30 = o->a.p.whole;
        o->d34 = o->y.p.whole;
        o->d38 = o->b.p.whole;
        k = D_80131148[Rand() & 0xf];
        if (k != 1) {
            if (k >= 2) break;
            if (k != 0) break;
            o->animFrame = 1;
            o->wba = ((Rand() & 1) << 5) + 0x20;
            o->wbc = 0x38;
            o->movetab = D_80077D0C;
            o->wac = 6;
            SETANIM(D_801345CC)
            asm("" ::: "memory");
            o->w22 = 0xb4;
            o->substep++;
        } else {
            o->animFrame = 0;
            o->wba = ((Rand() & 1) << 5) + 0x20;
            o->wbc = 0x38;
            o->movetab = D_80077D0C;
            o->wac = 6;
            SETANIM(D_801345CC)
            asm("" ::: "memory");
            o->w22 = 0xb4;
            o->substep++;
        }
        break;
    case 5: {
        register unsigned short pv asm("$3");
        if (o->animFrame) d = -0x20;
        else d = 0x20;
        if ((*(short*)&SPAD.s1c8)) {
            q = o->d38;
            pv = o->b.p.whole;
        } else {
            q = o->d30;
            pv = o->a.p.whole;
        }
        { register int w asm("$4") = U16(o->wba); asm("" : "=r"(w) : "0"(w)); pv = w + (pv - q); }
        if (--o->w22 == 0 || pv < U16(o->wba) * 2 || X(o)->wc0 != (*(short*)&SPAD.s1c8) ||
            FUN_800411cc(o, o->h->p.whole + d, o->y.p.whole + 0x18)) {
            o->w22 = 0x28;
            o->wac = 1;
            SETANIM(D_801345B8)
            o->substep++;
        }
        applyAnimVelocityX(o, o->animFrame);
        TICK
        break; }
    case 6:
        if (--o->w22) break;
        o->w22 = 0x78;
        o->wbc = 1;
        o->movetab = D_80077D84;
        o->wac = 1;
        SETANIM(D_801345B8)
        o->substep++;
        break;
    case 7:
        if (--o->w22 == 0) {
            if (NEAR(o)) {
                o->step = 1;
                o->state = 0;
                o->substep = 0;
            } else {
                o->movetab = D_80077D0C;
                o->wac = 6;
                o->animFrame ^= 1;
                SETANIM(D_801345CC)
                o->substep++;
            }
        }
        if (--U16(o->wbc) == 0) {
            o->wbc = 0x28;
            o->wac = 7;
            X(o)->wcc ^= 1;
            SETANIM(D_801345D0)
            if (X(o)->wca) SfxPlay(0x12);
        }
        applyAnimVelocityY(o, X(o)->wcc);
        break;
    case 8:
        if (o->animFrame) d = -0x20;
        else d = 0x20;
        if ((*(short*)&SPAD.s1c8))
            t2 = -o->d38 + U16(o->b.p.whole);
        else
            t2 = -o->d30 + U16(o->a.p.whole);
        if (({ register int x asm("$3") = t2 + 4; register unsigned r asm("$2") = (unsigned short)x; r < 8; }) || FUN_800411cc(o, o->h->p.whole + d, o->y.p.whole + 0x18)) {
            o->w22 = 0x3c;
            o->wac = 1;
            SETANIM(D_801345B8)
            o->substep++;
        }
        applyAnimVelocityX(o, o->animFrame);
        break;
    case 9:
        {
        int e;
        if ((*(short*)&SPAD.s1c8) == 0)
            e = X(o)->wc4 - U16(o->h->p.whole);
        else
            e = X(o)->wc8 - U16(o->h->p.whole);
        d = e + 8;
        }
        if ((unsigned short)d < 0x10) {
            o->timer = 300;
            o->substep++;
            break;
        }
        a.x = o->h->p.whole;
        if ((*(short*)&SPAD.s1c8) == 0)
            b.x = X(o)->wc4;
        else
            b.x = X(o)->wc8;
        a.y = o->y.p.whole;
        b.y = X(o)->wc6 - 0x38;
        if (a.x > b.x) {
            o->animFrame = 1;
            o->h->p.whole -= 2;
        } else {
            o->animFrame = 0;
            o->h->p.whole += 2;
        }
        o->y.raw += (short)FUN_8001fdac((angleBetweenPoints(a, b) + 0x100) & 0xf8, 0x200) << 8;
        TICK
        break;
    case 10:
        o->y.raw += ((X(o)->wc6 << 16) - o->y.raw) >> 6;
        if (--U16(o->wbc) == 0) {
            o->wbc = 0x28;
            if (X(o)->wca) SfxPlay(0x12);
        }
        if ((unsigned short)(X(o)->wc6 - o->y.p.whole + 2) < 4) {
            if (FUN_80041ebc(o, o->h->p.whole, o->y.p.whole + 0x18)) {
                o->wb4 = 0;
                o->state = D_80131128[Rand() & 0xf];
                o->substep = 0;
                if (o->d->p.whole == (*(Fix16 **)&PLAYER.obj.d)->p.whole) {
                    register int t3 asm("$3");
                    register Fix16 *hp asm("$4");
                    t3 = (*(unsigned short*)&PLAYER.obj.y.p.whole);
                    t3 -= U16(o->y.p.whole);
                    asm("" : "=r"(t3) : "0"(t3));
                    hp = o->h;
                    if ((unsigned short)(-hp->p.whole + (*(Fix16 **)&PLAYER.obj.h)->p.whole + 0x80) < 0x100 &&
                        ({ register unsigned r asm("$2") = (unsigned short)(t3 += 0x30); r < 0xf8; })) {
                        o->state = 4;
                        o->substep = 0;
                    }
                }
                break;
            }
        } else if (--o->timer) {
            break;
        }
        o->state = 3;
        o->substep = 0;
        break;
    }
}
