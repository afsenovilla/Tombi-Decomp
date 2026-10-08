// FUNC 8004b454 296 MAIN0
/* score 39 (ncheck, era 50): falla lbu b->bc antes del subu de d (juego: despues), t1/t2 intercambiados y orden addu h40->s2+t2. */
typedef struct { char p0[2]; unsigned short s2; } H;
typedef struct {
    char p0[0xc]; unsigned char bc;
    char p0b[0x16 - 0xd]; unsigned short s16;
    char p1[0x2e - 0x18]; unsigned short s2e;
    char p1b[0x40 - 0x30]; H *h40; H *h44;
    char p2[0x69 - 0x48]; unsigned char b69;
    char p2b[0x6c - 0x6a]; unsigned short s6c; short s6e; unsigned short s70; short s72;
    char p3[0x7e - 0x74]; short s7e;
    char p3b[0x9e - 0x80]; unsigned char b9e;
    char p4[0xb8 - 0x9f]; unsigned short sb8, sba;
    char p5[0xe8 - 0xbc]; unsigned short se8, sea;
} TO;
typedef struct E { unsigned short a, b; } E;
extern E DAT_8007b5e4[];

static __inline__ void f(TO *a, TO *b)
{
    char pad[8];
    unsigned short t1, t2;
    int d, e, w;
    unsigned v;
    unsigned short x;

    if ((unsigned short)(a->h44->s2 - b->h44->s2 + 0x2d) < 0x5b) {
        x = a->se8;
        d = x - a->h40->s2;
        t2 = DAT_8007b5e4[b->bc].a;
        t1 = DAT_8007b5e4[b->bc].b;
        if ((d << 16) >= 0) e = d; else e = -d;
        w = x - (b->h40->s2 + t2);
        if (a->s2e & 1)
            v = b->s6c + e + w;
        else
            v = b->s6c + w;
        if ((unsigned short)v <= b->s6e + (short)e
            && (unsigned short)(b->s70 + (a->sea - (b->s16 + t1))) <= b->s72) {
            a->b9e = 3;
            a->s7e = 0;
            a->sb8 = t2;
            a->sba = 0;
            b->b69 = 2;
            *(short *)0x1f80019e = 0;
            *(TO **)0x1f8003c0 = b;
        }
    }
}

void func_8004B454(TO *a, TO *b)
{
    f(a, b);
}
