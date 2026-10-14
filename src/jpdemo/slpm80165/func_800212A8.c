// JPDEMO slpm80165 800212A8 176
// Demo version of PAL main0 80020C04 (setSectionFlagBit.c): section sizes count double (PAL single).
extern unsigned short n1, n2;
extern unsigned short tabw[];
void setSectionFlagBit(int p)
{
    char pad;
    int i, s = 0, r;
    unsigned short *q;
    int *b;
    for (i = 0; i < (int)n1; i++)
        s += tabw[i] * 2;
    q = &n2;
    s += *q;
    if (p >= 0x20) s++;
    r = p % 32;
    b = (int *)((char *)q + 0x232);
    b[s] |= 1 << r;
}
