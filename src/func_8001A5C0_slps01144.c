// near-twin of PAL areaMode2Tick (second JP twin: no inventory-screen block, layer-7 update after the main objects, func_8002DA2C inside the script tick)
// VERSION SLPS_01144 slps01144 8001A5C0 368
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
// Ported from psx_tomba (gamestate.c, areaMode2Tick); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void func_8001A5C0(void)
{
    char* var_a0;
    u_short var_a1;
    Task* temp_v1;

    NEXT_PRIM = (int) ((FRAME_BUFFER_INDEX * 0xC000) + LOAD_BUFFER_ALT) & 0xFFFFFF;
    if ((*(short* )(&SCRATCHPAD+0x1C6) == 2) && (MOVIE_PLAY_STATE == MOVIE_STATE_IDLE)) {
        *(short* )0x1F8001C6 = 0;
    }
    updatePauseMenu();
    if (*(short* )(&SCRATCHPAD+0x1C6) == 0) {
        (*(u_short* )(&SCRATCHPAD+0x1F8))++;
        func_80034524();
        func_80029008();
        if (*(short* )0x1F8001C6 == 0) {
            func_8003C9D4();
            updateMainObjects();
            func_8002DA2C();
            updateObjectsLayer7();
            updateObjectsUnlayered();
            func_80055BA0();
        }
    }
    if (*(short* )(&VIEW_CAMERA[0]+0x106) != 1) {
        updateInventoryOverlay();
    }
    if (*(short* )(&SCRATCHPAD+0x1C6) == 0) {
        updateObjectsLayer8();
    }
    if (*(short* )0x1F8001C6 != 2) {
        func_80046264();
    } else {
        resetDrawLists();
    }
    updateSound();
}
#endif
