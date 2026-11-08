// VERSION SCUS_94236 scus94236 80019D78 240
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (main/system/boot.c, bootPlayIntroMovie); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"

// INCLUDE_ASM("nonmatchings/main/system/boot", bootPlayIntroMovie);
void bootPlayIntroMovie(void)
{
    u8 temp_v0;
    Task* task2;
    scratchpad* scratch = PSX_SCRATCH;
    Task* task = scratch->currentTask;
    
    switch (task->state1) {
        case 0:
            SetDispMask(0);
            initDisplay(0U, 0U, 0U);
            *(u8* )&scratch->moviePlayState = 1;
            *(s8* )&scratch->movieId = 0;
            scratch->currentTask->state1 ++;
            openTask(1, &moviePlayerTask);
            break;
        case 1:
            temp_v0 = *(u_long**)&scratch->moviePlayState;
            if (temp_v0 == 0) {
                task->state1++;
                return;
            }
            break;
        case 2:
            stopBgm(0);
            task2 = *(u_long**)&scratch->currentTask;
            task2->state0 = 4;
            task2->state1 = 0;
            break;
    }
    return;
}
#endif
