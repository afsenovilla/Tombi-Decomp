// FUNC 8004211c 528 MAIN0
/* score 28 (ncheck, with --expand-div). Left: (1) y0: game `lh v0; move t0,v0; addiu v0,v0,-16` (loaded temp for the
   -16 compare, copy kept in t0 for the two later uses); ours loads straight into t0. Tried: short/int ys + y0 = ys
   copies in 5 type combos (cse rewrites the compare to the copy, combine merges the load: no change), re-reading
   the field in each branch via DAT_1f800278[-3] (the stores to the global pointer invalidate it: real reloads, 50).
   (2) lo: game `lhu v0; andi v1,v0,0xf; srl v0; andi a2; beqz; nop` - with `unsigned short lo` the andi is early but
   an `andi 0xffff` appears at the compare; with `int lo` (single-set) sched1 gives it the birthing boost and sinks
   it to the beqz delay slot (dump: priority 7f000001). `lo = (a - lo) % t; t = y0 + h * lo / t;` (reusing lo for the
   remainder, as the game's v1 suggests) makes lo multi-set and reproduces the whole lo block, but lo becomes a
   global pseudo whose v1 preference (with h's) makes global-alloc skip v1 for the loop pointer (a3 instead): 37.
   n58 (still 28; dumps with cc1 -ds/-dS/-dl explain both): (1) y0: the game `move t0,v0` is a copy of the lh
   temp placed BEFORE `addiu v0,v0,-16`, which is what ties the temp and the -16 result in v0 (so reorg cannot drag
   the move into the delay slot). An explicit copy (`ys = *p++; d = ys - 16; y0 = ys;` + a dead trailing use
   `v = y0;` so cse make_regs_eqv keeps y0 as the canonical reg for the later uses) does produce the copy, but
   sched1 (backward list scheduler) boosts it (single-set y0, live at block end = birthing insn) and sinks it below
   the addiu; a dead `y0 = 0` at the top does not help (flow deletes it, reg_n_sets back to 1). Needs y0 with a
   real second set, or the copy with a lower luid than the addiu while cse still leaves the compare on the temp.
   (2) lo: `andi v1,v0,0xf` comes first in the game because lo is NOT boosted (multi-set); with any single-set form
   it is scheduled last (delay slot). `lo = v; lo &= 0xf;` leaves a combine `(use lo)` and changes nothing.
   `short m` + `int lo` gives the game sll/sra/slt compare; `unsigned short m` gives andi/sltu (27, wrong shape).
   n64: both shapes ARE reproducible debt-free with in-place updates (anti-dependences pin the order):
   `int ys = (short)*p++; y0 = ys; ys -= 16; if (b < ys) return 0;` + a dead trailing `lo = ys;` after the loop (makes
   ys the cse-canonical reg, n48) gives `lh v0; move t0,v0; addiu v0,v0,-16; slt`; `unsigned int v = *p++; lo = v & 0xf;
   v >>= 4; t = v & 0xf;` with `int lo` gives `andi v1,v0,0xf; srl v0,v0,4; andi a2` and no andi 0xffff. But then global
   alloc swaps n/y0 (t0/t1: n 8 refs/63 = 3809 vs y0 6/36 = 3333, game needs y0 first) and lo/m (a3/v1: the sign-extended
   m temp 6 refs/5 insns = 24000 beats lo 8/18 = 13333 and takes v1): score 30 (/tmp H.c form). Needs y0 a bit
   shorter-lived or n longer, and m's extension temp weaker (or lo stronger). */
#include "TOBJ.H"
extern short *func_8003F200(int a, int b);
extern unsigned short *DAT_1f800278;

static __inline__ int check(TObj *o, short a, short b)
{
    short *q;
    short n;
    unsigned short v;
    int t;
    short h;
    short m;
    int y0;
    unsigned short lo;

    q = func_8003F200(a, o->d->p.whole);
    DAT_1f800278 = (unsigned short *)(q + 1);
    n = *q;
    if (n == 0) return 0;
    do {
        t = *DAT_1f800278++;
        n--;
        if (t & 2) {
            if (!(t & 0x10)) goto take;
        }
        DAT_1f800278 += 3;
        continue;
take:
        y0 = (short)*DAT_1f800278++;
        if (b < y0 - 16) return 0;
        h = *DAT_1f800278++;
        if (h == 0) {
            DAT_1f800278++;
            t = y0;
        } else {
            v = *DAT_1f800278++;
            lo = v & 0xf;
            t = (v >> 4) & 0xf;
            if (t == 0) continue;
            m = a % 8;
            if (m < lo) continue;
            if (lo + t < m) continue;
            t = y0 + h * ((a - lo) % t) / t;
        }
        if ((short)(t - b) >= 0) return 1;
    } while (n != 0);
    return 0;
}

int func_8004211C(TObj *o, short a, int b)
{
    char pad[0x10];
    return check(o, a, b);
}
