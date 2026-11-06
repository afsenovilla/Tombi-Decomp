// FUNC 8001c364 720 MAIN0
// MATCHING 8001c364 720
// VERSION SLPS_01144 slps01144 8001BD78 572
// Portado de psx_tomba (gamestate.c, areaTransitionHandler); licencia MIT del proyecto original.
#if defined(VERSION_SLPS_01144)
// Portado de psx_tomba (gamestate.c, areaTransitionHandler); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"
/* Two extern views of GAME.selectedArea: the game reloads it for each use (one struct view would CSE the load). */
extern u8 D_80071694[];
extern u8* D_80071644[];
extern u8 D_8009E529;
extern u16 D_8009E93C, D_8009E93E;
extern short D_8009E52A;
extern void func_8001F2D0(void);
extern void func_8001FFFC(s32, s32, s32);
extern void loadAreaSoundBank(void);
extern void func_8001B808_slps01144(void);
extern u16 GAME_A asm("GAME"), GAME_B asm("GAME");

void func_8001BD78(void)
{
    Task* task = CURRENT_TASK;
    switch (task->step.value) {
    case 0:
        task->step.value++;
        GAME_KEEP_BGM = 1;
        D_8009E529 = 1;
        break;
    case 1:
    {
        gameConfig* gp;
        s32 areaChanged;
        s32 variant;
        s32 val;

        gp = &GAME;
        areaChanged = gp->selectedArea != D_8009E93C;
        {
            task->step.value = 2;

            if (areaChanged) {
                GAME_KEEP_BGM = 2;
                func_8001F2D0();
                (*(Task**)(&SCRATCHPAD + 0x1D4))->step.value = 3;
                asm("");
                if (D_8009E93C == 0) {
                    if ((u32)(D_8009E93E - 1) < 2) {
                        D_8009E529 = 1;
                    }
                } else if (D_8009E93C == 1) {
                    if (D_8009E93E == 1) {
                        D_8009E529 = 1;
                    }
                } else {
                    D_8009E529 = 0;
                }
            } else {
                if (D_80071694[gp->selectedArea]) {
                    GAME_KEEP_BGM = 2;
                    func_8001F2D0();
                }
            }
        }

        gp->selectedArea = gp->nextArea;
        gp->selectedSection = gp->nextSection;
        gp->selectedSpawnPoint = gp->nextSpawnPoint;

        variant = resolveAreaVariant() & 0xFF;

        {
            u16 area = GAME_A;
            s32 idx = area + (u16)AREA_VARIANT;
            u16 sec = GAME_SELECTED_SECTION;

            val = D_80071644[idx][sec];
            LOADED_SECTION = sec;
            *(short*)0x1F8001DE = 0;
            LOADED_AREA = area;
            LOADED_SPAWN_POINT = D_8009E52A;
            *(short*)0x1F8001DC = val;

            func_8001FFFC(idx, sec, variant | areaChanged);
        }
        loadAreaSoundBank();
        LOADING_SCREEN_STATE = 0;
        break;
    }
    case 2:
        func_8001B808_slps01144();
        break;
    case 3:
        func_8001B808_slps01144();
        break;
    }
}
#else
#define SKIP_ASM
#include "common.h"
#include "game.h"
/* Two extern views of GAME.selectedArea: the game reloads it for each use (one struct view would CSE the load). */
extern u16 GAME_A asm("GAME"), GAME_B asm("GAME");

void areaTransitionHandler(void)
{
    switch (CURRENT_TASK->step.value) {
    case 0:
        func_800222B8(9, 1);
        goto advance;
    case 1:
        if (*(u8*)0x1F8001CE == 0) break;
    advance:
        CURRENT_TASK->step.value++;
        break;
    case 2:
        CURRENT_TASK->step.value++;
        asm("");
        GAME_KEEP_BGM = 1;
        D_8009BCE9 = 1;
        break;
    case 3:
    {
        gameConfig* gp;
        s32 areaChanged;
        s32 variant;
        s32 val;

        gp = &GAME;
        areaChanged = gp->selectedArea != D_8009C0FC;
        {
            Task* task;
            task = CURRENT_TASK;
            task->step.value = 4;

            if (areaChanged) {
                GAME_KEEP_BGM = 2;
                func_80020FAC();
                (*(Task**)(&SCRATCHPAD + 0x1D4))->step.value = 5;
                asm("");
                if (D_8009C0FC == 0) {
                    if ((u32)(D_8009C0FE - 1) < 2) {
                        D_8009BCE9 = 1;
                    }
                } else if (D_8009C0FC == 1) {
                    if (D_8009C0FE == 1) {
                        D_8009BCE9 = 1;
                    }
                } else {
                    D_8009BCE9 = 0;
                }
            } else {
                if (gp->selectedArea == 2) {
                    if (GAME_SELECTED_SECTION + D_8009C0FE == 1) {
                        task->step.value = 5;
                    }
                }
            }
        }

        gp->selectedArea = gp->nextArea;
        gp->selectedSection = gp->nextSection;
        gp->selectedSpawnPoint = gp->nextSpawnPoint;

        variant = resolveAreaVariant() & 0xFF;

        val = D_80077084[GAME_A + (u16)AREA_VARIANT][GAME_SELECTED_SECTION];
        *(short*)0x1F8001DE = 0;
        LOADED_AREA = GAME_A;
        LOADED_SECTION = GAME_SELECTED_SECTION;
        LOADED_SPAWN_POINT = GAME_SELECTED_SPAWN_POINT;
        *(short*)0x1F8001DC = val;

        func_80021C24(GAME_A + (u16)AREA_VARIANT);

        if (func_8001DE24(areaChanged) != -1) {
            GAME_KEEP_BGM = 2;
        }

        func_80021CC8(GAME_B + (u16)AREA_VARIANT, GAME_SELECTED_SECTION, variant | areaChanged);
        func_8003C78C();
        startSoundTask();
        LOADING_SCREEN_STATE = 0;
        break;
    }
    case 5:
        displayLoadingScreen();
        break;
    }
}
#endif
