// FUNC 800f183c 108 D0SPSUB
// MATCHING 800f183c 108
typedef struct { short w0; unsigned short w2; } Obj;
extern unsigned short GAME, GAME_SELECTED_SECTION;
extern unsigned char D_8009CDE4;
extern Obj *PLAYER_H;

int subScreenCondRefreshingDrinkSpot(void)
{
    int r = 0;
    if (GAME == 0x10 && GAME_SELECTED_SECTION == 6 && D_8009CDE4 != 0 && D_8009CDE4 != 0xFF)
    {
        unsigned int t = PLAYER_H->w2 - 0x64;
        r = t < 0x78;
    }
    return r;
}
