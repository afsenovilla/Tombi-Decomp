// FUNC 8004fd6c 92 MAIN0
extern int DAT_1f8001e0;

int FUN_8004fd6c(unsigned *a, int b, int c, int d, unsigned e)
{
    int i = c * 4 + ((d << 16) >> 14);
    unsigned *p;
    unsigned t;
    if (i < 0)
        i = 0;
    p = (unsigned *)(i + b);
    if ((unsigned)((int)p - DAT_1f8001e0) < 0xca0) {
        t = *p;
        *p = (unsigned)a;
        *a = t | e;
        return 0;
    }
    return 1;
}
