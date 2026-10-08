// FUNC 80123e04 460 X000
typedef struct { unsigned short *anim_dummy; } Dm;
typedef struct {
    char p0[8]; unsigned short w08; char p1[0x1e - 0xa]; unsigned short w1e; char p2[2]; unsigned short *anim; char p3[4]; signed char pad;
} O0;
typedef struct {
    char p0[8]; unsigned short w08; char p1[5]; signed char f; char p2[0x1e - 0x10]; unsigned short w1e; char p3[2]; unsigned short *anim;
    char p4[0x3c - 0x28]; char *volatile d3c; char p5[0xb6 - 0x40]; unsigned short wb6, wb8, wba;
} O;
typedef struct { char p[0x3c]; char *d3c; } O2;
typedef struct {
    unsigned char p0[4]; unsigned char b4, b5, b6, b7; short s8, sa; unsigned short wc, we, w10, w12, w14, w16, w18, w1a, w1c, w1e, w20, w22, w24, w26;
} P;
extern int G70, G74, G1e0;
extern P *G164;
extern int FUN_8004fba8(O *, int *, int *);
extern void FUN_8004fd6c(P *, int, int, int, int);

void FUN_80123e04(O *o)
{
    short off;
    unsigned char *s1;
    P *p;
    int a1;
    off = *(short *)(o->d3c + (*o->anim << 2) + 2);
    s1 = (unsigned char *)(((O2 *)o)->d3c + off);
    if (FUN_8004fba8(o, &G70, &G74) == 0) {
        a1 = G70;
        p = G164;
        p->b7 = 0x2c;
        p->b4 = o->wb6;
        p->b5 = o->wb6;
        p->b6 = o->wb6;
        p->b7 |= 2;
        p->wc = *(unsigned short *)s1;
        p->w14 = *(unsigned short *)(s1 + 4);
        p->w1c = *(unsigned short *)(s1 + 8);
        p->w24 = *(unsigned short *)(s1 + 0xc);
        p->w16 = o->w1e;
        p->we = o->w08;
        p->s8 = a1 + (signed char)s1[0xe] - o->wb8;
        a1 >>= 16;
        p->sa = a1 + (signed char)s1[0xf] - o->wba;
        p->w10 = p->s8 + s1[0xa] + o->wb8 * 2 - 1;
        p->w12 = p->sa;
        p->w18 = p->s8;
        p->w1a = p->sa + s1[0xb] + o->wba * 2 - 1;
        p->w20 = p->w10;
        p->w22 = p->w1a;
        FUN_8004fd6c(p, G1e0 + 0x10, G74, o->f, 0x9000000);
        G164 = G164 + 1;
    }
}
