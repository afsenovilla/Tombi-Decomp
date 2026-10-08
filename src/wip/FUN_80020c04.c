// FUNC 80020c04 172 MAIN0
extern unsigned short n1, n2;
extern unsigned short tabw[];
static __inline__ void inl(int p)
{
    int i, s = 0;
    int *b;
    for (i = 0; i < (int)n1; i++)
        s += tabw[i];
    s += n2;
    if (p >= 0x20) s++;
    b = (int *)((char *)&n2 + 0x232) + s;
    *b |= 1 << (p % 32);
}
void FUN_80020c04(int p)
{
    char pad;
    inl(p);
}
