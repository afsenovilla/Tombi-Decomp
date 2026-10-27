// FUNC 8001a940 472 MAIN0
// MATCHING 8001a940 472
#include "globals.h"
extern int NEXT_PRIM;
extern short FRAME_BUFFER_INDEX;
extern short DAT_1f8001c6;
extern unsigned char MOVIE_PLAY_STATE;
extern unsigned char DAT_1f8003d0;
extern unsigned char MOVIE_ID;
extern unsigned short JOYPAD_STATE;
extern unsigned char MOVIE_SKIP_REQUEST;
extern unsigned short TICK_COUNT;
extern void FUN_8001ca58(void);
extern void updateAreaScreenEffect(void);
extern void tickHud(void);
extern void tickAreaScript(void);
extern void FUN_8001d36c(void);
extern void updateObjectsLayer7(void);
extern void updateObjectsUnlayered(void);
extern void processObjectCollisions(void);
extern void updateInventoryOverlay(void);
extern void callAreaModelLinkHook(void);
extern void updateObjectsLayer8(void);
extern void resetDrawLists(void);
extern void drawGameplayScene(void);
extern void FUN_8001dbdc(void);

void gameplayTick(void)
{
    unsigned char c;
    NEXT_PRIM = (int)&LOAD_BUFFER_ALT[FRAME_BUFFER_INDEX * 0xc000] & 0xffffff;
    if (DAT_1f8001c6 == 2) {
        if (MOVIE_PLAY_STATE == 0) {
            DAT_1f8001c6 = 0;
        } else if (DAT_1f8003d0 == 1) {
            c = MOVIE_ID;
            if ((c == 5 || (unsigned char)(c - 7) < 2) && (JOYPAD_STATE & 0x4008)) {
                MOVIE_SKIP_REQUEST = 1;
                DAT_1f8003d0 = 0;
            }
        }
    }
    FUN_8001ca58();
    if (DAT_1f8001c6 == 0) {
        TICK_COUNT++;
        updateAreaScreenEffect();
        tickHud();
        if (DAT_1f8001c6 == 0) {
            tickAreaScript();
            FUN_8001d36c();
            updateObjectsLayer7();
            updateObjectsUnlayered();
            processObjectCollisions();
        }
    }
    if (DAT_1f8001c6 != 1)
        updateInventoryOverlay();
    if (DAT_1f8001c6 == 0) {
        callAreaModelLinkHook();
        updateObjectsLayer8();
    }
    if (DAT_1f8001c6 != 2)
        drawGameplayScene();
    else
        resetDrawLists();
    FUN_8001dbdc();
}
