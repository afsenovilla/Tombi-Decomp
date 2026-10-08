// FUNC 80118bc0 460 X000
// MATCHING 80118bc0 460
typedef struct { short lo, hi; } HL;
typedef struct { char p0[2]; short s2; } H;
typedef struct { unsigned short a, b; } E;
typedef struct {
    char p0[3]; unsigned char b3, state, step; char p1[6]; unsigned char b0c; char p2[7];
    union { int i; HL s; } y; char p3[0x24 - 0x18]; int a24; char *a28; char p4[2]; unsigned short w2e;
    char p5[0x40 - 0x30]; H *h; char p6[0x82 - 0x44]; short vel;
} O;
extern char D_80077cf4[], D_80077cdc[];
extern E T_80138664[];
extern int T_8013b1d4[];
extern void FUN_8001fe6c(O *), FUN_8001faf4(O *), FUN_8001fec0(O *), FUN_800187e4(O *);
extern int FUN_800202b4(O *);
extern short FUN_80040278(O *, int, int);

void FUN_80118bc0(O *o)
{
    E *t;
    switch (o->state) {
    case 0:
        o->state++;
        o->vel = -0x400;
        o->a28 = (o->b3 & 1) ? D_80077cdc : D_80077cf4;
        t = &T_80138664[o->b3];
        o->w2e = t->a;
        o->b0c = t->b;
        o->a24 = T_8013b1d4[o->b0c];
        FUN_8001fe6c(o);
        break;
    case 1:
        if (FUN_800202b4(o) == 0) {
            o->state = 3;
            break;
        }
        switch (o->step) {
        case 0:
            o->vel += 0x40;
            o->y.i += o->vel << 8;
            if (o->vel > 0) o->step++;
            break;
        case 1:
            o->vel += 0x40;
            o->y.i += o->vel << 8;
            if (FUN_80040278(o, o->h->s2, o->y.s.hi))
                o->state = 3;
            break;
        }
        FUN_8001faf4(o);
        FUN_8001fec0(o);
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
