// FUNC 80020c04 172 MAIN0
extern unsigned short n1, n2;
extern unsigned short tabw[];
void FUN_80020c04(int p)
{
    int i = 0, s = 0;
    unsigned short *q;
    int *b;
    if (n1 != 0) {
        q = tabw;
        do {
            i++;
            s += *q++;
        } while (i < (int)n1);
    }
    s += n2;
    if (p >= 0x20) s++;
    b = (int *)((char *)&n2 + 0x232) + s;
    *b |= 1 << (p % 32);
}
