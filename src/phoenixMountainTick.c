// FUNC 8001acdc 472 MAIN0
// MATCHING 8001acdc 472
// Portado de psx_tomba (gamestate.c, phoenixMountainTick); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0x10];
    s16 unk12;
    s16 unk14;
    s16 unk16;
} unk_800A3940;
#define MENU_STATE ((unk_800A3940*)D_800A3940)

void phoenixMountainTick(void)
{
    char* var_a0;
    u_short var_a1;
    unkstruct_1F8001D4* temp_v1;

    NEXT_PRIM = (int) ((FRAME_BUFFER_INDEX * 0xC000) + &D_800B3188) & 0xFFFFFF;
    if ((*(short* )(&SCRATCHPAD+0x1C6) == 2) && (MOVIE_PLAY_STATE == MOVIE_IDLE)) {
        *(short* )0x1F8001C6 = 0;
    }
    func_8001D6C0();
    if (GAME.inventoryScreen == 0xFF) {
        GAME.inventoryScreen = 0;
        temp_v1 = CURRENT_TASK;
        var_a0 = *(u_short*)&temp_v1->state2;
        var_a1 = *(u_short*)&(CURRENT_TASK)->unk4E.value;
        D_800A3952 = 6;
        D_800A3954 = 0;
        D_800A3956 = 0;
        D_800A3940[0] = 0;
        temp_v1->state2 = 3U;
        (CURRENT_TASK)->unk4E.value = 0U;
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
            func_8001DFD4();
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
    func_8001F6D4();
}
