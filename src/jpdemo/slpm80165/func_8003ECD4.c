// JPDEMO slpm80165 8003ECD4 540
// Demo version of PAL main0 8004211C (probeFloorSegmentBelow.c): returns 0 for a negative column first.
/* Matched (was score 28). Keys: `short lo` (not int/unsigned short: no andi 0xffff and the sign-extended m temp
   no longer out-ranks lo for v1); n as `int` loaded through a short temp `nn` (`nn = *q; n = nn; if (nn == 0)`),
   which drops n to 7 weighted refs so y0 (6 refs/37 insns) is allocated before it (t0/t1); y0 as a copy of the
   int load temp `ys` (`y0 = ys; ys -= 16;`) with dead trailing `lo = ys; lo = nn;` so cse keeps the temps
   canonical (move t0,v0 before addiu v0,v0,-16); `unsigned int v` with in-place `v >>= 4`; frame via pad[0x18]. */
#include "TOBJ.H"
extern short *getCollisionPlaneAt(int a, int b);
extern unsigned short *DAT_1f800278;

static __inline__ int check(TObj *o, short a, short b)
{
    short *q;
    int nn;
    int n;
    unsigned int v;
    int t;
    short h;
    short m;
    int y0;
    short lo;
    int ys;

    if (a < 0) return 0;
    q = getCollisionPlaneAt(a, o->d->p.whole);
    DAT_1f800278 = (unsigned short *)(q + 1);
    nn = *q;
    n = nn;
    if (nn == 0) return 0;
    do {
        t = *DAT_1f800278++;
        n--;
        if (t & 2) {
            if (!(t & 0x10)) goto take;
        }
        DAT_1f800278 += 3;
        continue;
take:
        ys = (short)*DAT_1f800278++;
        y0 = ys;
        ys -= 16;
        if (b < ys) return 0;
        h = *DAT_1f800278++;
        if (h == 0) {
            DAT_1f800278++;
            t = y0;
        } else {
            v = *DAT_1f800278++;
            lo = v & 0xf;
            v >>= 4;
            t = v & 0xf;
            if (t == 0) continue;
            m = a % 8;
            if (m < lo) continue;
            if (lo + t < m) continue;
            t = y0 + h * ((a - lo) % t) / t;
        }
        if ((short)(t - b) >= 0) return 1;
    } while ((short)n != 0);
    lo = ys; t = nn;
    return 0;
}

int probeFloorSegmentBelow(TObj *o, short a, int b)
{
    char pad[0x18];
    return check(o, a, b);
}
