// FUNC 80058604 124 MAIN0
// MATCHING 80058604 124
extern signed char GAME_PLAYER_MODE;
extern unsigned char DAT_8009c93f;
extern unsigned char HUD_STATE[];
extern short DAT_800b1418;
extern void drawHud(void *);
void drawHudIfShown(void)
{
    unsigned char *p = HUD_STATE;
    if (GAME_PLAYER_MODE != 3) {
        if (DAT_8009c93f != 0)
            DAT_800b1418 = 0x78;
        if (DAT_800b1418 == 0 && (*p & 1) != 0)
            drawHud(p);
    }
}
