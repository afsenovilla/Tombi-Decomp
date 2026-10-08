// FUNC 8004fdc8 84 MAIN0
// MATCHING 8004fdc8 84
extern int DAT_1f8001e0;
int FUN_8004fdc8(unsigned *a, char *b, int c, int d, unsigned e)
{
    d = (short)d << 2;
    if (d < 0) d = 0;
    d += (int)b;
    if ((unsigned)(d - DAT_1f8001e0) >= 0xca0) return 1;
    {
        unsigned v = *(unsigned *)d;
        *(unsigned *)d = (unsigned)a;
        *a = v | e;
    }
    return 0;
}
