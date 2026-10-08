// FUNC 80103cbc 132 X016
// MATCHING 80103cbc 132
typedef struct { char p0[2]; unsigned short s2; } H;
typedef struct R {
    char p0[6]; char b6; char p1[0x16 - 7]; unsigned short s16; char p2[0x20 - 0x18]; short s20;
    char p3[0x40 - 0x22]; H *h; char p4[0x70 - 0x44]; unsigned short s70; char p5[0x7c - 0x72];
    short s7c, s7e; char p6[0x8c - 0x80]; unsigned char b8c;
} R;
typedef struct {
    char p0[6]; char b6; char p1[0x16 - 7]; unsigned short s16; char p2[0x20 - 0x18]; short s20;
    char p3[0x40 - 0x22]; H *h; char p4[0x7c - 0x44];
    short s7c, s7e; char p6[0x8c - 0x80]; unsigned int w8c; char p7[0x9c - 0x90]; char b9c; char p8[0xac - 0x9d]; char bac;
} TO;
extern R *X;
extern void g(TO *o, int a);

void FUN_80103cbc(TO *o)
{
    o->s20 = 0;
    g(o, 0xd);
    o->bac = 3;
    o->b9c = 0;
    {
        R *r = X;
        o->s7c = 0;
        o->s7e = 0;
        o->h->s2 = r->h->s2;
        o->s16 = r->s16 - r->s70;
        o->w8c = r->b8c;
        o->b6 = 2;
    }
}
