// FUNC 8004461c 192 MAIN0
// MATCHING 8004461c 192
typedef struct { char p0[2]; unsigned short s2; } H;
typedef struct {
    char p0[0x16]; unsigned short s16;
    char p1[0x40 - 0x18]; H *h40; H *h44;
    char p2[0x6c - 0x48]; unsigned short s6c; short s6e; unsigned short s70; short s72;
} TO;

int func_8004461C(TO *a, TO *b)
{
    if ((unsigned short)(a->h44->s2 - b->h44->s2 + 0x2d) >= 0x5b)
        return -1;
    if ((unsigned short)(a->h40->s2 - b->h40->s2 + (b->s6c + a->s6c)) > (b->s6e + a->s6e))
        return -1;
    if ((unsigned short)(a->s16 - b->s16 + (a->s70 + b->s70)) > (a->s72 + b->s72))
        return -1;
    return 1;
}
