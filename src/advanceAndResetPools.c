// FUNC 8001a0b4 60 MAIN0
// MATCHING 8001a0b4 60
// Ported from psx_tomba (gamestate.c, advanceAndResetPools); MIT licence of the original project.
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

void advanceAndResetPools(void)
{
    Task* temp_v1;

    temp_v1 = CURRENT_TASK;
    temp_v1->step.value++;
    initObjectPools();
    *(char* )0x1F8001CF = 0;
}
