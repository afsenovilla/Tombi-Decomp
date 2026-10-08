// FUNC 80134980 672 X000
typedef struct { short lo, hi; } HL;
typedef union { int i; HL s; } FX;
typedef struct { char p0[2]; short s2; } H;
typedef struct O {
    char p0[3]; unsigned char b3; char p1[2]; unsigned char b6; char p2[0x14 - 7]; FX y; char p3[8]; short s20; char p4[2];
    int anim; char p5[6]; unsigned short w2e; char p6[0x40 - 0x30]; H *h; char p7[0x7c - 0x44]; short vx, vy, s80, s82;
} O;
extern int A_8013ac14, A_8013ac44, A_8013ac2c;
extern H *DAT_800a6078[];
extern unsigned char D_8009c93f, D_8009c942, D_8009c93e, D_8009d00e;
extern void FUN_8001fe6c(O *), FUN_8001fec0(O *);

void FUN_80134980(O *o)
{
    switch (o->b6) {
    case 0:
        o->anim = A_8013ac14;
        FUN_8001fe6c(o);
        o->w2e = 1;
        if (DAT_800a6078[1]->s2 == 0) {
            if ((unsigned short)(DAT_800a6078[0]->s2 - o->h->s2 + 0x50) < 0xa0) {
                D_8009c93f = 1;
                D_8009c942 = 1;
                D_8009c93e = 1;
                o->b6++;
            }
        }
        break;
    case 1:
        o->anim = A_8013ac44;
        FUN_8001fe6c(o);
        o->vx = 0xc0;
        o->w2e = 0;
        o->vy = -0x480;
        o->b6++;
    case 2:
        FUN_8001fec0(o);
        *(int *)o->h += o->vx << 8;
        o->y.i += o->vy << 8;
        o->vy += 0x20;
        if (o->vy > 0) {
            o->anim = A_8013ac2c;
            FUN_8001fe6c(o);
            o->w2e = o->h->s2 > 0x138;
            o->s82 = (-0x11c - o->y.s.hi) >> 6;
            o->s80 = (0x138 - o->h->s2) >> 6;
            o->vy = -0x200;
            o->s20 = 0x40;
            o->b6++;
        }
        break;
    case 3:
        FUN_8001fec0(o);
        o->h->s2 += o->s80;
        o->y.s.hi += o->s82;
        o->y.i += o->vy << 8;
        o->vy += 0x10;
        if (--o->s20 <= 0) {
            o->anim = A_8013ac14;
            FUN_8001fe6c(o);
            o->w2e = 0;
            o->b3 = 0;
            o->h->s2 = 0x138;
            o->y.s.hi = -0x11c;
            D_8009d00e = 1;
            D_8009c93f = 0;
            D_8009c942 = 0;
            D_8009c93e = 0;
        }
        break;
    }
}
