// VERSION BETA_980203 beta980203 8004D140 120
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/func_8004B9F0_slps01144.c (func_8004B9F0_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
#define HUD_STATE D_800B2EE0_beta980203
#define GAME_PLAYER_MODE D_8009EE58_beta980203
#define DAT_8009c93f D_8009E4E7_beta980203
#define DAT_800b1418 D_800B2EE8_beta980203
#define drawHud func_8004BA68_beta980203
extern signed char GAME_PLAYER_MODE;
extern unsigned char DAT_8009c93f;
extern unsigned char HUD_STATE[];
extern short DAT_800b1418;
extern void drawHud(void *);
void func_8004D140_beta980203(void)
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
