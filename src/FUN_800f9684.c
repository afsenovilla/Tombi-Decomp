// FUNC 800f9684 548 X000
// MATCHING 800f9684 548
typedef struct { short s0, s2; } H;
typedef struct O {
    char p0[5]; unsigned char b5, b6; char p1; unsigned char b8, b9; char p2[0x16 - 0xa]; unsigned short w16;
    char p3[0x28 - 0x18]; unsigned short w28, w2a, w2c, w2e; char p4[0x40 - 0x30]; H *h;
    char p5[0x69 - 0x44]; unsigned char b69; char p6[0x8c - 0x6a]; unsigned int a8c; char p7[0x9e - 0x90]; unsigned char b9e;
    char p8[0xa4 - 0x9f]; unsigned char ba4; char p9[5]; unsigned char baa; char pa[5]; short sb0; char pb[0xbe - 0xb2]; unsigned char bbe;
} O;
extern O *DAT_8009c330;
extern unsigned char DAT_801152e8[];
extern short FUN_8003fd78(O *, int, int);
extern void FUN_800efc04(O *);
extern void FUN_8001fe94(O *, int);
extern void FUN_80041ca8(O *, int, int);

static __inline__ void setanim(O *o, unsigned short x)
{
    O *p = DAT_8009c330;
    p->w2c = x;
    if (p->w2e != x) {
        p->w2c = x;
        FUN_800efc04(o);
        FUN_8001fe94(o, 0);
        DAT_8009c330->w2e = DAT_8009c330->w2c;
    }
}

void FUN_800f9684(O *o)
{
    H *h;
    int t;
    short d;
    if (o->b69 != 0 || FUN_8003fd78(o, 0, 0)) {
        if (o->bbe & 8) {
            DAT_8009c330->b8 = o->w2e & 1;
            o->b5 = 0x1b;
            o->b6 = 0;
            if ((o->bbe & 1) != o->w2e) {
                setanim(o, 8);
                FUN_8001fe94(o, 2);
            } else {
                setanim(o, 0x11);
            }
        } else {
            o->ba4 = 0;
            o->baa = 0;
            o->b9e = 0;
            DAT_8009c330->w2e = 0xffff;
            DAT_8009c330->w28 = 0xffff;
            DAT_8009c330->w2a = 0xffff;
            o->a8c = DAT_801152e8[o->sb0];
            o->b5 = 0;
            o->b6 = 0;
        }
    } else {
        DAT_8009c330->b9 = 0;
        o->w16 += 9;
        h = o->h;
        t = h->s2;
        if (o->w2e & 1) d = t - 2; else d = t + 2;
        h->s2 = d;
        FUN_80041ca8(o, o->h->s2, (short)o->w16);
        setanim(o, 10);
        o->b5 = 0xd;
        o->b6 = 0;
    }
}
