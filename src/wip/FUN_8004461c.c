// FUNC 8004461c 192 MAIN0
typedef struct { char p0[2]; unsigned short s2; } H;
typedef struct {
    char p0[0x16]; unsigned short s16;
    char p1[0x40 - 0x18]; H *h40; H *h44;
    char p2[0x6c - 0x48]; unsigned short s6c; short s6e; unsigned short s70; short s72;
} TO;

static __inline__ int hit(TO *a, TO *b)
{
    int r = -1;
    if ((unsigned short)(a->h44->s2 - b->h44->s2 + 0x2d) < 0x5b) {
        if ((unsigned short)(a->h40->s2 - b->h40->s2 + (b->s6c + a->s6c)) > (b->s6e + a->s6e)) {
            r = -1;
        } else {
            r = -1;
            if ((unsigned short)(a->s16 - b->s16 + (b->s70 + a->s70)) <= (b->s72 + a->s72)) r = 1;
        }
    }
    return r;
}

int FUN_8004461c(TO *a, TO *b)
{
    return hit(a, b);
}
