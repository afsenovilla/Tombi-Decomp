// FUNC 8004b6a0 292 MAIN0
// w1: score 18; regs only (s6e lh into v0 + move t0, d in a2). Brute int/short/ushort of d,ad,v,w,e, reversed compares, local h=s6e: no gain.
typedef struct { char p0[2]; unsigned short s2; } H;
typedef struct {
    char p0[0x16]; unsigned short s16;
    char p1[0x2e - 0x18]; unsigned short s2e;
    char p1b[0x40 - 0x30]; H *h40; H *h44;
    char p2[0x6c - 0x48]; unsigned short s6c; short s6e; unsigned short s70; short s72;
    char p3[0xe8 - 0x74]; unsigned short se8; unsigned short sea;
} TO;
extern short DAT_1f8003bc;

int FUN_8004b6a0(TO *a, TO *b)
{
    char pad;
    int d, ad, v, w, e;

    if ((unsigned short)(a->h44->s2 - b->h44->s2 + 0x2d) >= 0x5b) return 0;
    e = a->se8;
    d = e - a->h40->s2;
    ad = d;
    if ((short)d < 0) ad = -d;
    w = e - b->h40->s2;
    if (a->s2e & 1) v = b->s6c + ad; else v = b->s6c;
    if ((unsigned short)(v + w) > b->s6e + (short)ad) return 0;
    d = b->s70 + (a->sea - b->s16);
    if (b->s72 < (unsigned short)d) return 0;
    if (!(a->s2e & 1)) DAT_1f8003bc = -b->s6c;
    else DAT_1f8003bc = b->s6e - b->s6c;
    return 1;
}
