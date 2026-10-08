// FUNC 8001fa88 44 MAIN0
// MATCHING 8001fa88 44
typedef struct P { short v; short w; } P;
typedef struct S {
    char pad0[0x28];
    P *tab;
    char pad1[0x40 - 0x2c];
    int *h;
} S;
void FUN_8001fa88(S *o, unsigned short i)
{
    P *p = &o->tab[i]; int *h = o->h; *h += p->v << 8;
}
