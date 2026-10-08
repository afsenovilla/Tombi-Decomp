// FUNC 80048ffc 176 MAIN0
// MATCHING 80048ffc 176
typedef struct H { short pad; unsigned short w; } H;
typedef struct O {
    char pad0[0x16];
    unsigned short y;
    char pad1[0x40 - 0x18];
    H *h;
    H *d;
    char pad2[0x6c - 0x48];
    unsigned short box0;
    short box1;
    unsigned short box2;
    short box3;
} O;

static __inline__ int Overlap(O *a, O *b)
{
    if ((unsigned short)(a->d->w - b->d->w + 0x2d) >= 0x5b) return 0;
    if ((unsigned short)((a->h->w - b->h->w) + (a->box0 + b->box0)) > a->box1 + b->box1) return 0;
    return (unsigned short)((a->y - b->y) + (a->box2 + b->box2)) <= a->box3 + b->box3;
}

int FUN_80048ffc(O *a, O *b)
{
    return Overlap(a, b);
}
