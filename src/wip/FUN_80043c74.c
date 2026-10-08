// FUNC 80043c74 268 MAIN0
typedef struct { char p0[2]; unsigned short s2; } H;
typedef struct {
    char p0[0x16]; unsigned short s16;
    char p1[0x40 - 0x18]; H *h40; H *h44;
    char p2[0x6c - 0x48]; unsigned short s6c; short s6e; unsigned short s70; short s72;
    char p3[0xa6 - 0x74]; char ba6;
} TO;

static __inline__ int f(TO *a, TO *b)
{
    char pad[16];
    int d, w, t, e0, e1;
    if ((unsigned short)(a->h44->s2 - b->h44->s2 + 0x2d) < 0x5b) {
        d = a->h40->s2 - b->h40->s2;
        w = b->s6c + a->s6c;
        e0 = b->s6e; e1 = a->s6e;
        if ((unsigned short)(d + w) <= e0 + e1
            && (unsigned short)(a->s16 - b->s16 + (b->s70 + a->s70)) <= a->s72 + b->s72) {
            if ((d << 16) < 0) t = -w;
            else t = (e0 - b->s6c) + (e1 - a->s6c);
            a->h40->s2 = b->h40->s2 + t;
            a->ba6 = (t << 16) < 0 ? 2 : 3;
            return 2;
        }
    }
    return 0;
}

int FUN_80043c74(TO *a, TO *b)
{
    return f(a, b);
}
