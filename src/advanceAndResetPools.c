// FUNC 8001a0b4 60 MAIN0
// MATCHING 8001a0b4 60
// Ported from psx_tomba (gamestate.c, advanceAndResetPools); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void advanceAndResetPools(void)
{
    Task* temp_v1;

    temp_v1 = CURRENT_TASK;
    temp_v1->step.value++;
    initObjectPools();
    *(char* )0x1F8001CF = 0;
}
