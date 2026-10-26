// FUNC 8001acdc 472 MAIN0
// MATCHING 8001acdc 472
// Ported from psx_tomba (gamestate.c, areaMode2Tick); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void areaMode2Tick(void)
{
    char* var_a0;
    u_short var_a1;
    Task* temp_v1;

    NEXT_PRIM = (int) ((FRAME_BUFFER_INDEX * 0xC000) + LOAD_BUFFER_ALT) & 0xFFFFFF;
    if ((*(short* )(&SCRATCHPAD+0x1C6) == 2) && (MOVIE_PLAY_STATE == MOVIE_STATE_IDLE)) {
        *(short* )0x1F8001C6 = 0;
    }
    updatePauseMenu();
    if (GAME.inventoryScreen == 0xFF) {
        GAME.inventoryScreen = 0;
        temp_v1 = CURRENT_TASK;
        var_a0 = *(u_short*)&temp_v1->state2;
        var_a1 = *(u_short*)&(CURRENT_TASK)->step.value;
        D_800A3952 = 6;
        D_800A3954 = 0;
        D_800A3956 = 0;
        MENU_STATE.unk0 = 0;
        temp_v1->state2 = 3U;
        (CURRENT_TASK)->step.value = 0U;
        *(short* )0x1F8003B8 = (short)var_a0;
        *(u_short* )0x1F8003BA = var_a1;
    }
    if (*(short* )(&SCRATCHPAD+0x1C6) == 0) {
        (*(u_short* )(&SCRATCHPAD+0x1F8))++;
        func_80034524();
        func_80029008();
        if (*(short* )0x1F8001C6 == 0) {
            func_8003C9D4();
            updateObjectsLayer7();
            updateMainObjects();
            updateObjectsUnlayered();
            func_80055BA0();
        }
    }
    if (*(short* )(&D_1F8000C0[0]+0x106) != 1) {
        updateInventoryOverlay();
    }
    if (*(short* )(&SCRATCHPAD+0x1C6) == 0) {
        func_8002DA2C();
        updateObjectsLayer8();
    }
    if (*(short* )0x1F8001C6 != 2) {
        func_80046264();
    } else {
        resetDrawLists();
    }
    updateSound();
}
