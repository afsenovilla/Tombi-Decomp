// FUNC 8001b9b0 324 MAIN0
// MATCHING 8001b9b0 324
#include "globals.h"
extern short FRAME_BUFFER_INDEX;
extern short DRAW_FROZEN;
extern unsigned short TICK_COUNT;
extern unsigned int NEXT_PRIM;
extern void FUN_8001ca58(void);
extern void updateAreaScreenEffect(void);
extern void FUN_8001d36c(void);
extern void updateObjectsUnlayered(void);
extern void FUN_800edeec(void);
extern void updateInventoryOverlay(void);
extern void updateObjectsLayer8(void);
extern void FUN_800ed4d0(void);
extern void updateObjectsLayer7(void);
extern void resetDrawLists(void);
extern void drawGameplayScene(void);
extern void SoundUpdate(void);

void areaMode5Tick(void)
{
    NEXT_PRIM = (unsigned int)(LOAD_BUFFER_ALT + FRAME_BUFFER_INDEX * 0xc000) & 0xffffff;
    FUN_8001ca58();
    if (DRAW_FROZEN == 0) {
        TICK_COUNT = TICK_COUNT + 1;
        updateAreaScreenEffect();
        if (DRAW_FROZEN == 0) {
            FUN_8001d36c();
            updateObjectsUnlayered();
            FUN_800edeec();
        }
    }
    if (DRAW_FROZEN != 1)
        updateInventoryOverlay();
    if (DRAW_FROZEN == 0) {
        updateObjectsLayer8();
        if (DRAW_FROZEN == 0) {
            FUN_800ed4d0();
            updateObjectsLayer7();
        }
    }
    if (DRAW_FROZEN != 2)
        drawGameplayScene();
    else
        resetDrawLists();
    SoundUpdate();
}
