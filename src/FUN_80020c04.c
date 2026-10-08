// FUNC 80020c04 172 MAIN0
// MATCHING 80020c04 172
extern unsigned short n1, n2;
extern unsigned short tabw[];
void FUN_80020c04(int p)
{
    char pad;
    int i, s = 0, r;
    unsigned short *q;
    int *b;
    for (i = 0; i < (int)n1; i++)
        s += tabw[i];
    q = &n2;
    s += *q;
    if (p >= 0x20) s++;
    r = p % 32;
    b = (int *)((char *)q + 0x232);
    b[s] |= 1 << r;
}
