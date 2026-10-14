// FUNC 8011cec8 924 X001
// MATCHING 8011cec8 924
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
    register int b asm("$17");
    register int h asm("$2");
    int d;
    int e;
    int k;
    register int v asm("$2");
    int f0;
    int kp;
    int a2;
    int a3;
    int b2;
    int w;
    short t;
    short dx;
    short dy;

    f0 = c->f0; a = ((c->f2 - f0) * s->w20) / c->f2;
    w = s->w10;
    kp = c->f0 * w;
    b = kp / c->f2;
    a2 = (kp * (100 - a) / 100) / c->f2;
    n = &D_800A4470[c->f4 + 1];
    dy = n->x2 - s->x2;
    dx = n->x0 - s->x0;
    h = (0x800 - w) / 2;
    t = h;
    b2 = h + b;
    a3 = h + a2;
    dx = (dx << 12) / rcos(t);
    d = rcos(t) - rcos((short)b2);
    k = dx / 2;
    c->fa = (d * k) / 4096;
    e = rsin(t) - rsin((short)a3);
    e = -((e * k) / 4096);
    if (c->f6 == c->f4) {
        if (c->f0 < c->f8) v = ((c->f0 - c->f8) << 6) / c->f8;
        else { register int q asm("$3"); q = ((c->f0 - c->f8) << 6); q = q / (c->f2 - c->f8); v = q; }
        e += MulCosDup(v & 0xff, s->w16);
    }
    c->fc = (c->f0 * dy) / c->f2 + ((short)e * s->w12) / 100;
    c->fe = 0;
}
