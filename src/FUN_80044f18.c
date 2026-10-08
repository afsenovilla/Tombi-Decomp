// FUNC 80044f18 464 MAIN0
// MATCHING 80044f18 464
typedef struct FP { unsigned short frac; short whole; } FP;
typedef struct O { unsigned char b0; char p0[0x10-1]; FP a; FP y; FP b; unsigned char b1c; char p1[0x40-0x1d];
  FP *h; FP *d; char p2[0x68-0x48]; unsigned char b68; char p3; unsigned char b6a; char p4;
  short box0, box1, box2, box3; char p5[42]; unsigned char b9e, b9f; char p6[0xa8-0xa0]; short wa8; } O;
extern short DAT_1f80019e;
extern int FUN_800425c4(O *, O *);
extern void FUN_8001f96c(int, int, int, int);
extern void SfxPlay(int);

static __inline__ short hit(O *a, O *b)
{
    short d;
    if ((unsigned short)(a->d->whole - b->d->whole + 0x2d) >= 0x5b)
        return -1;
    d = a->h->whole - b->h->whole;
    if ((unsigned short)(d + (b->box0 + a->box0)) > b->box1 + a->box1)
        return -1;
    if ((unsigned short)((a->y.whole - b->y.whole) + (a->box2 + b->box2)) > a->box3 + b->box3)
        return -1;
    DAT_1f80019e = 0;
    if (d < 0)
        return 0;
    return 1;
}

void FUN_80044f18(O *a, O *b)
{
    int r;
    if (hit(a, b) < 0)
        return;
    r = 1;
    b->b68 = 1;
    b->b9e = 0;
    switch (FUN_800425c4(a, b)) {
    case 1: case 7:
        r = 1;
        break;
    case 4: case 5: case 10:
        r = 1;
        a->b6a = 1;
        a->b0 = 2;
        a->wa8 = 0x4ff;
        break;
    case 0: case 3: case 9:
        r = 2;
        break;
    case 13:
        b->b68 = 0;
        r = -1;
        break;
    case 6: case 11: case 12:
        r = 2;
        a->b6a = 1;
        a->b0 = 2;
        a->wa8 = 0x4ff;
        break;
    case 2: case 8:
        r = 0;
        b->b9e = 1;
        b->b9f = 0;
        break;
    }
    if (r < 0)
        return;
    FUN_8001f96c(1, a->a.whole, a->y.whole, a->b.whole);
    SfxPlay((b->b1c & 0x7f) == 4 ? 6 : 7);
}
