// FUNC 80020a78 116 MAIN0
extern unsigned short DAT_8009c960;
extern unsigned DAT_8009c994[][8];

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
    return DAT_8009c994[t][q] & (1 << r);
}
