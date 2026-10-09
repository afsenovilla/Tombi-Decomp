// FUNC 8011cec8 924 X001
/* score 192: second half (rcos/rsin, MulCosDup branch, final fc/fe) matches; first part differs: game computes
   b = (f0*w10)/f2 right after the product (mflo s1) and keeps f2 in a0, the table base in a3 (s[1] via +0x22);
   ours schedules b's div after the (100 - a)/100 chain, keeps f2 in s1 and needs fp (9 saved regs).
   Tried: statement order, recomputing the product, w10 in a local.
   o27: s[1] is a separate pointer n = &D_800A4470[c->f4 + 1] (game: table base kept in a3, +0x22 added later;
   score 193 but the base/offset code now matches). Hill-climbed the first 12 statements (no change: sched1
   places b's div (insn 64, last use of k and f2) after a's; game keeps k live after div b). */
typedef struct {
    short x0, x2, x4, x6, x8, xa, xc, n;
    short w10, w12, w14, w16, w18;
    unsigned char b1a, b1b;
    short w1c, w1e, w20;
} Seg;

typedef struct {
    short f0, f2, f4, f6, f8, fa, fc, fe;
} Ctx;

extern Seg D_800A4470[];
extern int rcos(int);
extern int rsin(int);
extern int MulCosDup(int, int);

void func_8011CEC8(Ctx *c)
{
    Seg *s = &D_800A4470[c->f4];
    Seg *n;
    int a, b, h, d, e, k, v, w;
    short t, dx, dy;

    a = ((c->f2 - c->f0) * s->w20) / c->f2;
    w = s->w10;
    k = c->f0 * w;
    b = k / c->f2;
    a = (k * (100 - a) / 100) / c->f2;
    n = &D_800A4470[c->f4 + 1];
    dy = n->x2 - s->x2;
    dx = n->x0 - s->x0;
    h = (0x800 - w) / 2;
    t = h;
    b = h + b;
    a = h + a;
    dx = (dx << 12) / rcos(t);
    d = rcos(t);
    d -= rcos((short)b);
    k = dx / 2;
    c->fa = (d * k) / 4096;
    e = rsin(t);
    e -= rsin((short)a);
    e = -((e * k) / 4096);
    if (c->f6 == c->f4) {
        if (c->f0 < c->f8) v = ((c->f0 - c->f8) << 6) / c->f8;
        else v = ((c->f0 - c->f8) << 6) / (c->f2 - c->f8);
        e += MulCosDup(v & 0xff, s->w16);
    }
    c->fe = 0;
    c->fc = (c->f0 * dy) / c->f2 + ((short)e * s->w12) / 100;
}
