// FUNC 8001fab4 64 MAIN0
typedef struct P { short v; short w; } P;
typedef struct S { char pad0[0x14]; int y; char pad1[0x28-0x18]; P *tab; short pad2; unsigned short idx; char pad3[0x40-0x30]; int *h; } S;
void FUN_8001fab4(S *o)
{
register P *e = o->tab + o->idx; *o->h += e->v << 8; o->y += e->w << 8;
}
