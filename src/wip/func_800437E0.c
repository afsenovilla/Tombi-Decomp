// FUNC 800437e0 532 MAIN0
// wip score 129: logic right; game keeps copies (t0=sw, t6=dx, t7=dy, t4/t5 = s6e) and recomputes sh+dy for the <0xc test; maybe an inline with params
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
    char pad[16];
    int w6c, w6a, sw, dx, adx, h6e, a6e, dy, sh, pen, k;

    if ((unsigned short)(a->h44->s2 - b->h44->s2 + 0x2d) >= 0x5b) return 0;
    w6c = b->s6c;
    w6a = a->s6c;
    sw = w6c + w6a;
    pen = sw;
    dx = a->h40->s2 - b->h40->s2;
    adx = dx;
    h6e = b->s6e;
    a6e = a->s6e;
    if ((unsigned short)(dx + sw) > h6e + a6e) return 0;
    dy = a->s16 - b->s16;
    sh = b->s70 + a->s70;
    if ((unsigned short)(dy + sh) > a->s72 + b->s72) return 0;
    if ((unsigned short)(sh + dy) < 0xc) {
        if (a->b9c & 1) return 0;
        a->sb0 = 0;
        goto land;
    }
    if ((short)dx < 0) {
        k = pen;
        adx = -dx;
        pen = -sw;
    } else {
        pen = (h6e - w6c) + (a6e - w6a);
        k = pen;
    }
    if ((unsigned short)(k - adx) < 4) {
        a->h40->s2 = b->h40->s2 + pen;
        if ((short)pen < 0) a->ba6 = 2; else a->ba6 = 3;
        return 2;
    }
    if ((short)dy > 0) {
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
