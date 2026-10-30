// near-twin of drawHudIfShown (PAL)
// VERSION SLPS_01144 slps01144 8004B9F0 120
#if defined(VERSION_SLPS_01144)
#define HUD_STATE D_800B2EE0
#define GAME_PLAYER_MODE D_8009EE58
#define DAT_8009c93f D_8009E4E7
#define DAT_800b1418 D_800B2EE8
#define drawHud func_8004BA68
extern signed char GAME_PLAYER_MODE;
extern unsigned char DAT_8009c93f;
extern unsigned char HUD_STATE[];
extern short DAT_800b1418;
extern void drawHud(void *);
void func_8004B9F0_slps01144(void)
{
    unsigned char *p = HUD_STATE;
    if (GAME_PLAYER_MODE != 3) {
        if (DAT_8009c93f != 0)
            DAT_800b1418 = 0x78;
        if (DAT_800b1418 == 0 && *p != 0)
            drawHud(p);
    }
}
#endif
