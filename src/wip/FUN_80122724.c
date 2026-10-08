// FUNC 80122724 408 X000
typedef struct { char p[2]; short h; } Hd;
typedef struct {
    char p0[1]; unsigned char vis; char p1[2]; unsigned char st; char p2[7]; unsigned char b0c, b0d, p3, b0f; char p4[2];
    short a; unsigned short y; short b; char p5[2]; short w1e; char p6[4]; int anim; char p7[8]; int d30, d34;
    char p8[4]; int d3c; Hd *h;
} O;
extern short G176;
extern unsigned short G186;
extern int G2d4;
extern int T13b20c[];
extern void FUN_80018da4(O *), FUN_80018934(O *);

void FUN_80122724(O *q)
{
    int v, t, x;
    short s;
    O *o = q;
    char pad[8];
    unsigned char c = o->st;
    switch (c) {
    case 0:
        o->w1e = 0xc;
        o->b0d = 0;
        o->y = o->y + 0x180;
        o->anim = T13b20c[o->b0c];
        o->b0f = 0;
        o->d3c = G2d4;
        o->d34 = (short)o->y;
        o->st = o->st + 1;
        o->d30 = o->h->h;
        break;
    case 1:
        v = G176;
        if (v < 0x719) break;
        if (o->b0c != 0) {
            t = o->d30 - v;
            s = (t << 16) >> 19;
        } else {
            t = o->d30 - v;
            x = t << 16;
            if ((x >> 16) < -0x200)
                s = (t & 0xfff) >> 3;
            else
                s = x >> 19;
            if (s >= 0x180) break;
        }
        o->a = s;
        o->b = 0;
        o->vis = 1;
        o->y = ((o->d34 - G186) & 0xfff) >> 4;
        FUN_80018da4(o);
        break;
    case 2:
        o->st = c + 1;
        break;
    case 3:
        FUN_80018934(o);
        break;
    }
}
