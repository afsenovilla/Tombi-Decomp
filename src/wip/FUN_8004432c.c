// FUNC 8004432c 248 MAIN0
typedef struct { char p0[2]; unsigned short s2; } H;
typedef struct {
    char t0, t1, t2, t3, t4, t5, t6, t7; char p0[0x16 - 8]; unsigned short s16;
    char p1[0x40 - 0x18]; H *h40; H *h44;
    char p2[0x6a - 0x48]; char b6a; char b6b; unsigned short s6c; short s6e; unsigned short s70; short s72;
    char p3[0xb4 - 0x74]; unsigned short sb4;
} TO;

static __inline__ int hit(TO *a, TO *b)
{
    int r = 0;
    if ((unsigned short)(a->h44->s2 - b->h44->s2 + 0x2d) < 0x5b
        && (unsigned short)(a->h40->s2 - b->h40->s2 + (a->s6c + b->s6c)) <= (a->s6e + b->s6e)
        && (unsigned short)(a->s16 - b->s16 + (a->s70 + b->s70)) <= (a->s72 + b->s72)) r = 1;
    return r;
}

void FUN_8004432c(TO *a, TO *b)
{
    if (b->t2 != 0xb || b->sb4 != 1) {
        if (hit(a, b)) {
            a->b6a = 1;
            b->t0 = 4;
            b->t4 = 2;
            b->t5 = 0;
            b->t6 = 0;
        }
    }
}
