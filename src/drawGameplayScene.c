// FUNC 8005042c 280 MAIN0
// MATCHING 8005042c 280
extern void FUN_80063e0c(void *);
extern void drawPlayer(void), drawCategory1Objects(void), drawCategory2Objects(void), drawCategory3Objects(void);
extern void drawCategory4Objects(void), drawCategory5Objects(void), drawAreaModelList(void), drawCategory8Objects(void);
extern void drawCategory7Objects(void), drawPauseMenu(void), FUN_8011bc34(void), drawHudIfShown(void);
extern void drawScreenFade(int);
extern char DAT_1f800118[];
extern short DAT_1f8001c6;
extern unsigned char DAT_8009c975, DAT_8009c976;
extern int DAT_8009c960;

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
    switch (DAT_8009c975) {
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
    if (DAT_8009c960 == 6)
        FUN_8011bc34();
    else
        drawHudIfShown();
}
