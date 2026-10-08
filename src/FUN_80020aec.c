// FUNC 80020aec 120 MAIN0
// MATCHING 80020aec 120
extern unsigned short DAT_8009c960;
extern unsigned int DAT_80099994[][8];

void FUN_80020aec(int bit)
{
    int w, b;
    unsigned int k;

    w = bit / 32;
    b = bit % 32;
    if (DAT_8009c960 == 0x10)
        k = 7;
    else if ((k = DAT_8009c960) == 0x11)
        k = 0xc;
    DAT_80099994[k][w] |= 1 << b;
}
