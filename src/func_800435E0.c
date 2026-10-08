// FUNC 800435e0 512 MAIN0
// MATCHING 800435e0 512
typedef struct { char p0[2]; unsigned short s2; } H;
typedef struct {
    char p00[0x16]; unsigned short s16;
    char p1[0x2e - 0x18]; unsigned short s2e;
    char p1b[0x40 - 0x30]; H *h40; H *h44;
    char p3[0x6c - 0x48]; unsigned short s6c; short s6e; unsigned short s70; short s72;
    char p6[0xac - 0x74]; unsigned char bac;
} TO;

int func_800435E0(TO *a, TO *b)
{
    short w, k, sw, dx, dy, sh, dxs, dys, ex, ey;

    k = a->h44->s2 - b->h44->s2 + 0x2d;
    if ((unsigned short)k >= 0x5b) return -1;
    if (b->s2e & 1) w = b->s6e - b->s6c; else w = b->s6c;
    k = a->bac == 1;
    sw = k + (w + a->s6c);
    dx = a->h40->s2 - b->h40->s2;
    if ((unsigned short)(dx + sw) > b->s6e + a->s6e + k * 2) return -1;
    dy = a->s16 - b->s16;
    sh = b->s70 + a->s70;
    if ((unsigned short)(dy + sh) > a->s72 + b->s72) return -1;
    dxs = dx;
    if (dx < 0) {
        dx = -dx;
    } else {
        if (b->s2e & 1) w = b->s6c; else w = b->s6e - b->s6c;
        sw = k + (w + (a->s6e - a->s6c));
    }
    ex = sw - dx;
    dys = dy;
    if (dy < 0) {
        dy = -dy;
    } else {
        sh = (a->s72 - a->s70) + (b->s72 - b->s70);
    }
    ey = sh - dy;
    if (dys < 6) {
        if (ex >= ey) return 2;
    } else if (ex >= ey) {
        goto three;
    }
    return dxs >= 0;
three:
    return 3;
}
