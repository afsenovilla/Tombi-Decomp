// FUNC 8004b6a0 292 MAIN0
// score 14: ad/v short and ad reused for the last sum fixed the a2 part. Left: s6e lh lands in v1 (game v0) with the sum in the same reg.
// Tried: types brute force (729 combos x late var), compare operand orders, local h (int/short/ushort, early/late), static inline wrapper (move a3,a0 + 8 B frame suggest an inline; same output), nested ifs, result var.
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
    int d;
    short ad;
    short v;
    int w;
    int e;

    if ((unsigned short)(a->h44->s2 - b->h44->s2 + 0x2d) >= 0x5b) return 0;
    e = a->se8;
    d = e - a->h40->s2;
    ad = d;
    if ((short)d < 0) ad = -d;
    w = e - b->h40->s2;
    if (a->s2e & 1) v = b->s6c + ad; else v = b->s6c;
    if ((unsigned short)(v + w) > b->s6e + (short)ad) return 0;
    ad = b->s70 + (a->sea - b->s16);
    if (b->s72 < (unsigned short)ad) return 0;
    if (!(a->s2e & 1)) DAT_1f8003bc = -b->s6c;
    else DAT_1f8003bc = b->s6e - b->s6c;
    return 1;
}
