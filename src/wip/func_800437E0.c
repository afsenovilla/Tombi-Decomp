// FUNC 800437e0 532 MAIN0
// wip score 82 (was 129): rewritten like func_800482EC with short temps + "dy = d" copy before the 2nd test. Left: the <0xc test sign-extends d (game adds raw t2+a1), plus small reg diffs (t8/t9 vs t7/t8).
typedef struct { char p0[2]; unsigned short s2; } H;
typedef struct {
    char p0[0x14]; unsigned short s14; unsigned short s16;
    char p1[0x40 - 0x18]; H *h40; H *h44;
    char p2[0x69 - 0x48]; unsigned char b69;
    char p2b[0x6c - 0x6a]; unsigned short s6c; short s6e; unsigned short s70; short s72;
    char p3[0x7e - 0x74]; short s7e;
    char p4[0x9c - 0x80]; unsigned char b9c;
    char p5[0xa6 - 0x9d]; unsigned char ba6;
    char p6[0xb0 - 0xa7]; unsigned short sb0;
} TO;

int func_800437E0(TO *a, TO *b)
{
    char pad;
    short dx;
    short wx;
    short px;
    short dy;
    short d;
    int hy;
    short cx;

    if ((unsigned short)(a->h44->s2 - b->h44->s2 + 0x2d) >= 0x5b) return 0;
    wx = b->s6c + a->s6c;
    px = wx;
    dx = a->h40->s2 - b->h40->s2;
    if ((unsigned short)(dx + wx) > b->s6e + a->s6e) return 0;
    d = a->s16 - b->s16;
    hy = b->s70 + a->s70;
    dy = d;
    if ((unsigned short)(d + hy) > a->s72 + b->s72) return 0;
    if (((unsigned)(hy + d) & 0xffff) < 0xc) {
        if (a->b9c & 1) return 0;
        a->sb0 = 0;
        goto land;
    }
    cx = px;
    if (dx < 0) {
        dx = -dx;
        px = -px;
    } else {
        px = (b->s6e - b->s6c) + (a->s6e - a->s6c);
        cx = px;
    }
    if ((unsigned short)(cx - dx) < 4) {
        a->h40->s2 = b->h40->s2 + px;
        if (px < 0) a->ba6 = 2; else a->ba6 = 3;
        return 2;
    }
    if (dy > 0) {
        a->s16 = b->s16 + ((b->s72 - b->s70) + (a->s72 - a->s70));
        if (a->s7e < 0) a->s7e = 0;
        return 3;
    }
    if (a->b9c & 1) return 0;
    a->sb0 = 0;
land:
    a->s14 = 0;
    a->b69 = 1;
    a->s7e = 0;
    a->s16 = b->s16 - (b->s70 + a->s70);
    b->b69 = 3;
    if (a->b9c & 2) b->b69 = 1;
    return 1;
}
