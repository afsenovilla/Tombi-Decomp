// FUNC 8004211c 528 MAIN0
// best: matchcheck score 30 (ncheck expands div differently, use matchcheck). Left: y0 lh into v0 + copy to t0, v lhu into v0.
#include "TOBJ.H"
extern short *func_8003F200(int a, int b);
extern unsigned short *DAT_1f800278;

static __inline__ int check(TObj *o, short a, short b)
{
    short *q;
    short n;
    unsigned short v;
    int t;
    short h, m;
    int y0;
    int lo;

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
