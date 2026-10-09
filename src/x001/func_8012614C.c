// FUNC 8012614c 428 X001
// MATCHING 8012614c 428
typedef struct { char p0[2]; unsigned short s2; } H;
typedef struct {
    char p00[0x16]; unsigned short s16;
    char p1b[0x40 - 0x18]; H *h40; H *h44;
    char p3[0x6c - 0x48]; unsigned short s6c; short s6e; unsigned short s70; short s72;
    char p6[0xac - 0x74]; unsigned char bac;
} TO;
typedef struct {
    char p00[0x16]; unsigned short s16;
    char p1b[0x40 - 0x18]; H *h40; H *h44;
    unsigned short s48; short s4a; char p4c[2]; unsigned short s4e; short s50;
} TB;

int func_8012614C(TO *a, TB *b)
{
    short k, sw, dx, dy, sh, dxs, dys, ex, ey;

    k = a->h44->s2 - b->h44->s2 + 0x2d;
    if ((unsigned short)k >= 0x5b) return -1;
    k = a->bac == 1;
    sw = k + (b->s48 + a->s6c);
    dx = a->h40->s2 - b->h40->s2;
    if ((unsigned short)(dx + sw) > b->s4a + a->s6e + k * 2) return -1;
    dy = a->s16 - b->s16;
    sh = b->s4e + a->s70;
    if ((unsigned short)(dy + sh) > a->s72 + b->s50) return -1;
    dxs = dx;
    if (dx < 0) {
        dx = -dx;
    } else {
        sw = k + ((b->s4a - b->s48) + (a->s6e - a->s6c));
    }
    ex = sw - dx;
    dys = dy;
    if (dy < 0) {
        dy = -dy;
    } else {
        sh = (a->s72 - a->s70) + (b->s50 - b->s4e);
    }
    ey = sh - dy;
    if (dys < 9) {
        if (ex >= ey) return 2;
        return dxs >= 0;
    }
    if (ex >= ey) return 3;
    return dxs >= 0;
}
