// FUNC 80037fbc 104 MAIN0
// MATCHING 80037fbc 104
int FUN_80037fbc(char *o, int i)
{
    unsigned short *p;
    int n;
    int *q;
    if (o[0x88] != 0)
        return 1;
    p = (unsigned short *)(i * 2 + (int)o);
    if (*p == 0)
        return 2;
    o[0x88] = 1;
    n = 0x4f;
    q = (int *)(o + 0x13c);
    *(short *)(o + 0x8a) = *p - 1;
    *(short *)(o + 0x8c) = 0;
    do {
        q[0x1090 / 4] = 0;
        n--;
        q--;
    } while (n >= 0);
    return 0;
}
