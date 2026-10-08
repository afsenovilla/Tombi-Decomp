// FUNC 800f4ed0 324 X000
typedef struct G { char p0[1]; unsigned char b1; short w2; char p1[4]; unsigned char b8; char p2[5]; short we; } G;
typedef struct H { char p0[2]; short w2; } H;
typedef struct O {
    char p0[0x16]; short y;
    char p1[0x2e - 0x18]; unsigned short af;
    char p2[0x30 - 0x30]; int d30; int d34;
    char p3[0x40 - 0x38]; H *h;
    char p4[0x84 - 0x44]; int d84; int d88; int d8c;
} O;
extern G *DAT_80096330;
extern int FUN_8001fddc(int, int);
extern int FUN_8001fdac(int, int);

void FUN_800f4ed0(O *o, short a, int b)
{
    int d;
    int v;
    int s;
    if (o->af & 1) {
        s = 0x1bf;
        s -= b;
        s &= 0xff;
        o->h->w2 = FUN_8001fddc(s, DAT_80096330->b1) + o->d30;
        v = FUN_8001fdac(s, DAT_80096330->b1);
        o->d84 = 0;
        o->d8c = 0x100 - (a << 2);
        v += o->d34;
    } else {
        s = b + 0xc0;
        s &= 0xff;
        o->h->w2 = FUN_8001fddc(s, DAT_80096330->b1) + o->d30;
        v = FUN_8001fdac(s, DAT_80096330->b1);
        o->d84 = 0;
        o->d8c = a << 2;
        v += o->d34;
    }
    o->y = v;
    d = DAT_80096330->we;
    if (DAT_80096330->b8)
        d = d - DAT_80096330->w2;
    else
        d = d + DAT_80096330->w2;
    DAT_80096330->we = d;
}
