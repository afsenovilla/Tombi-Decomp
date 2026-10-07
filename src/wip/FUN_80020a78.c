// FUNC 80020a78 116 MAIN0
extern unsigned short DAT_8009c960;
extern char DAT_8009c994[];

unsigned FUN_80020a78(int n)
{
    int q = n / 32;
    int r = n % 32;
    unsigned t;
    t = DAT_8009c960;
    if (t == 0x10)
        t = 7;
    else if (t == 0x11)
        t = 0xc;
    return *(unsigned *)(DAT_8009c994 + t * 0x20 + q * 4) & (1 << r);
}
