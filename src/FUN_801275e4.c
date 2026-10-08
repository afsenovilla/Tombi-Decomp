// FUNC 801275e4 316 X000
// MATCHING 801275e4 316
typedef struct P { short w0; unsigned short w2; short w4; } P;
typedef struct H { char p0[2]; short x; } H;
typedef struct O {
    char p0[3]; unsigned char subtype;
    char p1[0x16 - 4]; short y;
    char p2[0x2e - 0x18]; unsigned short af;
    char p3[0x40 - 0x30]; H *h;
    char p4[0x9d - 0x44]; unsigned char b9d;
    char p5[0xb4 - 0x9e]; P p;
} O;
extern short DAT_8007a1f0[], DAT_8007a5f0[];
extern short FUN_8004065c(O *, int, int, int);

int FUN_801275e4(O *o)
{
    unsigned short a, b;
    P *p = &o->p;
    if (o->subtype == 9 && o->af == 1 && o->y < -0x64 && o->h->x < 0x446)
        return 2;
    if ((o->b9d & 2) && o->af == (o->b9d & 1))
        return 1;
    if (o->af == 0)
        p->w4 = 0x10;
    else
        p->w4 = -0x10;
    if (p->w2 != 0) {
        int i = (p->w2 & 0xff) * 2;
        a = (unsigned)(p->w4 * *(short *)((char *)DAT_8007a1f0 + i)) >> 12;
        b = (unsigned)(p->w4 * *(short *)((char *)DAT_8007a5f0 + i)) >> 12;
    } else {
        a = 0;
        b = p->w4;
    }
    return FUN_8004065c(o, (short)(o->h->x + b), (short)(o->y + a), (short)o->af) != 0;
}
