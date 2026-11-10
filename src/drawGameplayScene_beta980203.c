// VERSION BETA_980203 beta980203 80045328 304
#if defined(VERSION_BETA_980203)
// Beta version: USA C of src/drawGameplayScene.c: the area-6 HUD branch only in sections 0-1.
#define DRAW_FROZEN D_1F8001C6
#define LIGHTING D_1F800118
extern void FUN_80063e0c(void *);
extern void drawPlayer(void), drawCategory1Objects(void), drawCategory2Objects(void), drawCategory3Objects(void);
extern void drawCategory4Objects(void), drawCategory5Objects(void), drawAreaModelList(void), drawCategory8Objects(void);
extern void drawCategory7Objects(void), drawPauseMenu(void), FUN_8011bc34(void), drawHudIfShown(void);
extern void drawScreenFade(int);
extern char LIGHTING[];
extern short DRAW_FROZEN;
extern unsigned char GAME_FADE_SCREEN_CONTROL, GAME_FADE_SCREEN_AMOUNT;
extern unsigned short GAME[];

void drawGameplayScene(void)
{
    int u;
    FUN_80063e0c(LIGHTING);
    drawPlayer();
    drawCategory1Objects();
    drawCategory2Objects();
    drawCategory3Objects();
    drawCategory4Objects();
    drawCategory5Objects();
    drawAreaModelList();
    drawCategory8Objects();
    drawCategory7Objects();
    if (DRAW_FROZEN == 1)
        drawPauseMenu();
    switch (GAME_FADE_SCREEN_CONTROL) {
    case 1:
    case 4:
        u = 0xff;
        break;
    case 2:
        u = 0xff;
        u -= GAME_FADE_SCREEN_AMOUNT;
        break;
    case 3:
    case 0x10:
        u = GAME_FADE_SCREEN_AMOUNT;
        break;
    default:
        goto end;
    }
    drawScreenFade(u);
end:
    if (GAME[0] == 6 && GAME[1] < 2)
        FUN_8011bc34();
    else
        drawHudIfShown();
}
#endif
