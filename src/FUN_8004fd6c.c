// FUNC 8004fd6c 92 MAIN0
// MATCHING 8004fd6c 92
extern int DAT_1f8001e0;
int FUN_8004fd6c(unsigned *a, int b, int c, int d, unsigned e)
{
    unsigned t;
    c = c * 4 + ((d << 16) >> 14);
    if (c < 0)
        c = 0;
    c += b;
    if ((unsigned)(c - DAT_1f8001e0) >= 0xca0)
        return 1;
    t = *(unsigned *)c;
    *(unsigned *)c = (unsigned)a;
    *a = t | e;
    return 0;
}
