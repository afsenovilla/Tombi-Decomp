// VERSION SCUS_94236 scus94236 800199B8 748
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (main/system/boot.c, bootLogoState); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"

// INCLUDE_ASM("nonmatchings/main/system/boot", bootLogoState);
void bootLogoState(void)
{
    u16 state2;
    u16 timer;
    u16 state1;
    Task* task;
    Task* task2;
    Task* task3;
    Task* task4;
    Task* task5;
    Task* task6;
    Task* task7;
    Task* task8;

    state1 = (CURRENT_TASK)->state1;
    switch (state1) {
        case 0:
            task3 = CURRENT_TASK;
            state2 = task3->state2;
            switch (state2) {
                case 0:
                    SetDispMask(0);
                    LOAD_COMPLETE = 0U;
                    loadSoundSet(1);
                    queueSystemLoadList(1, 1);
                    task4 = SCRATCHPAD.currentTask;
                    task4->state2++;
                    return;
                case 1:
                    if (LOAD_COMPLETE != 0) {
                        task3->state1 = 2U;
                        task3->state2 = 0U;
                        func_800E7D5C();
                        return;
                    }
                default:
                    return;
            }
            break;
        case 1:
            SetDispMask(0);
            func_800E7D5C();
            task5 = SCRATCHPAD.currentTask;
            task5->state1++;
            return;
        case 2:
            task2 = CURRENT_TASK;
            MOVIE_PLAY_STATE = MOVIE_STATE_STARTING;
            MOVIE_ID = 0x15;
            task2->state1++;
            openTask(1, moviePlayerTask);
            return;
        case 3:
            if (MOVIE_PLAY_STATE != MOVIE_STATE_IDLE) {
                return;
            }
            task5 = SCRATCHPAD.currentTask;
            task5->state1++;
            return;
        case 4:
            SetDispMask(0);
            initDisplayHiRes(240U, 240U, 240U);
            SetDispMask(1);
            NEXT_PRIM = (s32) ((FRAME_BUFFER_INDEX * 0xC000) + &LOAD_BUFFER_ALT) & 0xFFFFFF;
            func_800E7D74();
            task = CURRENT_TASK;
            task->timer = 120;
            task->state1++;
            return;
        case 5:
            NEXT_PRIM = (s32) ((s32) ((FRAME_BUFFER_INDEX * 0xC000) + &LOAD_BUFFER_ALT) & 0xFFFFFF);
            func_800E7D74();
            task6 = CURRENT_TASK;
            timer = task6->timer - 1;
            task6->timer = timer;
            if ((timer << 0x10) <= 0) {
                stopBgm(0);
                SetDispMask(0);
                initDisplay(0U, 0U, 0U);
                LOAD_COMPLETE = 0U;
                loadSoundSet(2);
                queueSystemLoadList(2, 1);
                task5 = SCRATCHPAD.currentTask;
                task5->state1++;
                return;
            }
            break;
        case 6:
            if (LOAD_COMPLETE != 0) {
                task7 = CURRENT_TASK;
                task7->state1++;
                cdSeekStream(0);
                return;
            }
            break;
        case 7:
            task8 = CURRENT_TASK;
            task8->state0 = 3;
            task8->state1 = 0U;
            break;
    }
}
#endif
