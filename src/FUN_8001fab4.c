// FUNC 8001fab4 64 MAIN0
// MATCHING 8001fab4 64
typedef struct P { short v; short w; } P;
typedef struct S { char pad0[0x14]; int y; char pad1[0x28-0x18]; P *tab; short pad2; unsigned short idx; char pad3[0x40-0x30]; int *h; } S;
void FUN_8001fab4(S *o)
{
    P *e;
    int d;
    e = o->tab + o->idx;
    d = e->v << 8;
    *o->h += d;
    d = e->w << 8;
    o->y += d;
}
