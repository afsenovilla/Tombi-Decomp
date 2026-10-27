// FUNC 8005042c 280 MAIN0
// MATCHING 8005042c 280
// VERSION SLPM_80165 slpm80165 8003FAC4 264
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8005042C (drawGameplayScene.c): extra D_8009B160 test, always the normal HUD.
extern void FUN_80063e0c(void *);
extern void FUN_80050544(void), FUN_80050ea4(void), FUN_80054f54(void), FUN_80055174(void);
extern void drawCategory4Objects(void), drawCategory5Objects(void), FUN_80057d04(void), drawCategory8Objects(void);
extern void FUN_80057edc(void), FUN_800597bc(void), FUN_8011bc34(void), drawHudIfShown(void);
extern void FUN_80059d44(int);
extern char DAT_1f800118[];
extern short DAT_1f8001c6;
extern unsigned char GAME_FADE_SCREEN_CONTROL, DAT_8009c976;
extern int GAME;

extern signed char D_8009B160;
void drawGameplayScene(void)
{
    int u;
    FUN_80063e0c(DAT_1f800118);
    FUN_80050544();
    FUN_80050ea4();
    FUN_80054f54();
    FUN_80055174();
    drawCategory4Objects();
    drawCategory5Objects();
    FUN_80057d04();
    drawCategory8Objects();
    FUN_80057edc();
    if (DAT_1f8001c6 == 1 && D_8009B160 == 1)
        FUN_800597bc();
    switch (GAME_FADE_SCREEN_CONTROL) {
    case 1:
    case 4:
        u = 0xff;
        break;
    case 2:
        u = 0xff;
        u -= DAT_8009c976;
        break;
    case 3:
    case 0x10:
        u = DAT_8009c976;
        break;
    default:
        goto end;
    }
    FUN_80059d44(u);
end:
    drawHudIfShown();
}
#else
extern void FUN_80063e0c(void *);
extern void drawPlayer(void), drawCategory1Objects(void), drawCategory2Objects(void), drawCategory3Objects(void);
extern void drawCategory4Objects(void), drawCategory5Objects(void), drawAreaModelList(void), drawCategory8Objects(void);
extern void drawCategory7Objects(void), drawPauseMenu(void), FUN_8011bc34(void), drawHudIfShown(void);
extern void drawScreenFade(int);
extern char DAT_1f800118[];
extern short DAT_1f8001c6;
extern unsigned char GAME_FADE_SCREEN_CONTROL, DAT_8009c976;
extern int GAME;

void drawGameplayScene(void)
{
    int u;
    FUN_80063e0c(DAT_1f800118);
    drawPlayer();
    drawCategory1Objects();
    drawCategory2Objects();
    drawCategory3Objects();
    drawCategory4Objects();
    drawCategory5Objects();
    drawAreaModelList();
    drawCategory8Objects();
    drawCategory7Objects();
    if (DAT_1f8001c6 == 1)
        drawPauseMenu();
    switch (GAME_FADE_SCREEN_CONTROL) {
    case 1:
    case 4:
        u = 0xff;
        break;
    case 2:
        u = 0xff;
        u -= DAT_8009c976;
        break;
    case 3:
    case 0x10:
        u = DAT_8009c976;
        break;
    default:
        goto end;
    }
    drawScreenFade(u);
end:
    if (GAME == 6)
        FUN_8011bc34();
    else
        drawHudIfShown();
}
#endif
