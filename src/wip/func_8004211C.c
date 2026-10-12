// FUNC 8004211c 528 MAIN0
/* score 28 (ncheck, with --expand-div). Left: (1) y0: game `lh v0; move t0,v0; addiu v0,v0,-16` (loaded temp for the
   -16 compare, copy kept in t0 for the two later uses); ours loads straight into t0. Tried: short/int ys + y0 = ys
   copies in 5 type combos (cse rewrites the compare to the copy, combine merges the load: no change), re-reading
   the field in each branch via DAT_1f800278[-3] (the stores to the global pointer invalidate it: real reloads, 50).
   (2) lo: game `lhu v0; andi v1,v0,0xf; srl v0; andi a2; beqz; nop` - with `unsigned short lo` the andi is early but
   an `andi 0xffff` appears at the compare; with `int lo` (single-set) sched1 gives it the birthing boost and sinks
   it to the beqz delay slot (dump: priority 7f000001). `lo = (a - lo) % t; t = y0 + h * lo / t;` (reusing lo for the
   remainder, as the game's v1 suggests) makes lo multi-set and reproduces the whole lo block, but lo becomes a
   global pseudo whose v1 preference (with h's) makes global-alloc skip v1 for the loop pointer (a3 instead): 37. */
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
