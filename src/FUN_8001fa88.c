// FUNC 8001fa88 44 MAIN0
typedef struct P { short v; short w; } P;
typedef struct S {
    char pad0[0x28];
    P *tab;
    char pad1[0x40 - 0x2c];
    int *h;
} S;
void FUN_8001fa88(S *o, unsigned short i)
{
int *h = o->h; int v = o->tab[i].v << 8; *h += v;
}
