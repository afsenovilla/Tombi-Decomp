// near-twin of PAL areaMode4Tick (tools/jp_autofix.py)
// VERSION SLPS_01144 slps01144 8001B6D8 304
#if defined(VERSION_SLPS_01144)
#if defined(VERSION_SLPS_01144)
#include "globals.h"
extern short FRAME_BUFFER_INDEX, G1c6;
extern unsigned short TICK_COUNT;
extern unsigned int NEXT_PRIM;
extern void FUN_8001ca58(void), updateAreaScreenEffect(void), FUN_8001d36c(void), updateObjectsUnlayered(void), FUN_800ec5e8(void);
extern void tickAreaScript(void);
extern void updateInventoryOverlay(void), FUN_800ebc30(void), updateObjectsLayer8(void), updateObjectsLayer7(void);
extern void resetDrawLists(void), drawGameplayScene(void), FUN_8001dbdc(void);
extern void updateMainObjects(void);
extern void func_8002BEC4_slps01144(void);
extern void func_800ED8CC(void);
void func_8001B6D8(void)
{
    NEXT_PRIM = (unsigned int)(LOAD_BUFFER_ALT + FRAME_BUFFER_INDEX * 0xc000) & 0xffffff;
    FUN_8001ca58();
    if (G1c6 == 0) {
        TICK_COUNT = TICK_COUNT + 1;
        updateAreaScreenEffect();
        if (G1c6 == 0) {
            updateMainObjects();
            func_8002BEC4_slps01144();
            updateObjectsLayer8();
            updateObjectsUnlayered();
            func_800ED8CC();
        }
    }
    if (G1c6 != 1)
        updateInventoryOverlay();
    if (G1c6 == 0) {
        updateObjectsLayer7();
    }
    if (G1c6 != 2)
        drawGameplayScene();
    else
        resetDrawLists();
    FUN_8001dbdc();
}
#endif
#endif
