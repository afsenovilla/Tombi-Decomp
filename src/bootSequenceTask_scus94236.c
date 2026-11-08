// VERSION SCUS_94236 scus94236 800191E0 1132
// FLAGS -O2 -G0
// HEADERS tomba26
#if defined(VERSION_SCUS_94236)
// Portado de psx_tomba (main/system/boot.c, bootSequenceTask); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"

// INCLUDE_ASM("nonmatchings/main/system/boot", bootSequenceTask);
void bootSequenceTask(void)
{
    u16 state;
    Task* task5;
    Task* task6;
    Task* task7;
    Task* task8;
    Task* task;
    Task* task2;
    Task* task3;
    Task* task4;
    Task* task9;

    D_8009D6DD = 0;
    D_8009D6DE = 0;
    D_8009D6DF = 0;
    D_8009E3ED = 0;
    D_8009E3EE = 0;
    D_8009E3EF = 0;
    (*(s8 *)&SPAD->inTitle) = 0;
    (*(s8 *)&SPAD->inGame) = 0;
    BUTTON_CONFIG_INDEX = 0;
    D_8009C9E4 = 1;
    D_8009E450 = 0;
    loadButtonConfig();
    task = CURRENT_TASK;
    task->state0 = 9U;
    task->state1 = 0U;
    task->state2 = 0;
    SetDispMask(0);
    do {
        state = (CURRENT_TASK)->state0;
        switch (state) {
            case 0:
                LOAD_COMPLETE = 0;
                loadSoundSet(0);
                loadSoundSet(1);
                queueSystemLoadList(0, 1);
                (*(s8 *)&SPAD->unk1C4) = 0;
                (*(s8 *)&SPAD->unk1C5) = 0;
                memset(D_1F8001A0, 0, sizeof(D_1F8001A0));
                task9 = CURRENT_TASK;
                task9->state0++;
                break;
            case 1:
                if (LOAD_COMPLETE != 0) {
                    cdSeekStream(0x15);
                    (CURRENT_TASK)->state0 = 4U;
                }
                break;
            case 2:
                SetDispMask(1);
                task5 = CURRENT_TASK;
                task5->timer = 120;
                task5->state0++;
                break;
            case 3:
                task7 = CURRENT_TASK;
                NEXT_PRIM = (s32) ((FRAME_BUFFER_INDEX * 0xC000) + &LOAD_BUFFER_ALT) & 0xFFFFFF;
                task7->timer--;
                if ((s16) task7->timer == -1) {
                    task7->state0++;
                }
                fontDebugPrintf(0x50, 0x60, 0, &D_80010000);
                fontDebugPrintf(0x50, 0x70, 0, &D_80010008);
                break;
            case 4:
                task2 = CURRENT_TASK;
                task2->state0 = 0U;
                task2->state1 = 1U;
                task2->state2 = 0;
                setTask((s32*)titleSequenceTask);
                break;
            case 9:
                SetDispMask(0);
                *(byte* )0x1F8001CE = 0U;
                queueSystemLoadList(0x5E, 1);
                (CURRENT_TASK)->state0 = 0xAU;
                break;
            case 10:
                if (LOAD_COMPLETE != 0) {
                    SetDispMask(0);
                    initDisplayHiRes(0U, 0U, 0U);
                    SetDispMask(1);
                    task3 = CURRENT_TASK;
                    task3->timer = 240;
                    task3->state0 = 0xBU;
                    task3->state1 = 0U;
                }
                break;
            case 11:
                task8 = CURRENT_TASK;
                NEXT_PRIM = (s32) ((s32) ((FRAME_BUFFER_INDEX * 0xC000) + &LOAD_BUFFER_ALT) & 0xFFFFFF);
                switch (task8->state1) {
                    case 0:
                        task8->unk5A = 1U;
                        task8->state1++;
                        break;
                    case 1:
                        task8->unk5A++;
                        if ((s16) task8->unk5A >= 0x80) {
                            task8->timer = 0xB4U;
                            task8->state1++;
                            break;
                        }
                        break;
                    case 2:
                        task8->timer--;
                        if ((s16) task8->timer == -1) {
                            task8->state1++;
                        }
                        break;
                    case 3:
                        task8->unk5A--;
                        if ((s16)task8->unk5A == 0) {
                            SetDispMask(0);
                            initDisplay(0U, 0U, 0U);
                            task4 = SCRATCHPAD.currentTask;
                            *(byte* )0x1F8001D1 = 0;
                            (*(s16 *)&SPAD->frameCount) = 0;
                            task4->state0 = 0U;
                            task4->state1 = 0U;
                        }
                        break;
                }
                task6 = CURRENT_TASK;
                if (*(u16*)&task6->state0 == 0xB) {
                    drawBootLogo((u8) task6->unk5A, 1);
                }
                break;
        }
        sleepTask(1);
    } while(true);
}
#endif
