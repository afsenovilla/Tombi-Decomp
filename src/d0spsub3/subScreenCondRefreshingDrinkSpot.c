// FUNC 800ee95c 108 D0SPSUB3
// MATCHING 800ee95c 108
typedef struct { short w0; unsigned short w2; } Obj;
extern unsigned short GAME, D_8009C962;
extern unsigned char D_8009CDE4;
extern Obj *D_800A6078;

int subScreenCondRefreshingDrinkSpot(void)
{
    int r = 0;
    if (GAME == 0x10 && D_8009C962 == 6 && D_8009CDE4 != 0 && D_8009CDE4 != 0xFF)
    {
        unsigned int t = D_800A6078->w2 - 0x64;
        r = t < 0x78;
    }
    return r;
}
