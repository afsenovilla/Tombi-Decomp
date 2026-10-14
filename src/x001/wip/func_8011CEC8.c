// FUNC 8011cec8 924 X001
/* score 98 (was 192): asm("" : "=r"(b) : "0"(b)) after b = k / f2 (153), then greedy pins d=$22, k=$5, h=$30.
   Left: game loads f0 before f2, keeps f2 in a0 and puts b's div right after the product; prologue saves.
   Earlier: tried statement order, recomputing the product, w10 in a local, n = &D_800A4470[c->f4 + 1]. */
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
    int a;
    int b;
    register int h asm("$30");
    register int d asm("$22");
    int e;
    register int k asm("$5");
    int v;
    int w;
    short t;
    short dx;
    short dy;

    a = ((c->f2 - c->f0) * s->w20) / c->f2;
    w = s->w10;
    k = c->f0 * w;
    b = k / c->f2;
    asm("" : "=r"(b) : "0"(b));
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
