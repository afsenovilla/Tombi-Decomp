// FUNC 80020aec 120 MAIN0
extern unsigned short DAT_8009c960;
extern unsigned int DAT_80099994[];
void FUN_80020aec(int bit)
{
    int w = bit / 32;
    int b = bit % 32;
    unsigned int k;
    unsigned int *p;
    k = DAT_8009c960;
    if (k == 0x10)
        k = 7;
    else if (k == 0x11)
        k = 0xc;
    p = (unsigned int *)((char *)DAT_80099994 + k * 32) + w;
    *p |= 1 << b;
}
