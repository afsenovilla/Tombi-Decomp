// JPDEMO slpm80165 80019C94 252
// Demo version of PAL main0 80019F98 (gameplayStateDispatcher.c): no mode 5 handler and no game-over mode 8.
extern unsigned short *TASK_C;
extern short DAT_1f8001dc;
extern short DAT_1f8001de;
extern unsigned char D_1F8001CE;
extern void FUN_8001a43c(void), FUN_8001a64c(void), areaMode2Handler(void), inventoryScreenHandler(void), areaMode4Handler(void), areaMode5Handler(void), areaMode6Handler(void), FUN_8001c364(void), transitionToGameOver(void);
void gameplayStateDispatcher(void)
{
    short *p;
    short a, b;
    switch (TASK_C[0x4c / 2]) {
    case 0: FUN_8001a43c(); break;
    case 1: FUN_8001a64c(); break;
    case 2: areaMode2Handler(); break;
    case 3: inventoryScreenHandler(); break;
    case 4: areaMode4Handler(); break;
    case 6: areaMode6Handler(); break;
    case 7: FUN_8001c364(); break;
    }
    a = DAT_1f8001dc;
    if (-1 < a && D_1F8001CE != 0) {
        DAT_1f8001dc = -1;
        TASK_C[0x4c / 2] = a;
        TASK_C[0x4e / 2] = DAT_1f8001de;
    }
}
