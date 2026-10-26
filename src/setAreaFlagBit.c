// FUNC 80020aec 120 MAIN0
// MATCHING 80020aec 120
extern unsigned short GAME;
extern unsigned int DAT_8009c994[][8];

void setAreaFlagBit(int bit)
{
    int w, b;
    unsigned int k;

    w = bit / 32;
    b = bit % 32;
    if (GAME == 0x10)
        k = 7;
    else if ((k = GAME) == 0x11)
        k = 0xc;
    DAT_8009c994[k][w] |= 1 << b;
}
