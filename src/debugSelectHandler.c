// FUNC 8001a43c 528 MAIN0
// MATCHING 8001a43c 528
// VERSION SLPS_01144 slps01144 8001A108 432
// Portado de psx_tomba (gamestate.c, debugSelectHandler); licencia MIT del proyecto original.
#if defined(VERSION_SLPS_01144)
// Portado de psx_tomba (gamestate.c, debugSelectHandler); licencia MIT del proyecto original.
#define SKIP_ASM
extern void drawDebugText(int, int, int, char *);
extern char D_80010184[];
#include "common.h"
#include "game.h"

void func_8001A108(void)
{
    int var_a0;
    Task* p;
    u_char* temp1;
    u_char* temp2;

    switch ((CURRENT_TASK)->step.value) {
        case 0:
            (CURRENT_TASK)->step.value++;
            initObjectPools();
            *(char* )0x1F8001CF = 0;
            return;
        case 1:
            displayDebugScreen();
            return;
        case 3:
            func_80020FAC();
            (*(Task**)(&SCRATCHPAD+0x1D4))->step.value++;
            initObjectPools();
            *(u_char*)&(*(u_long**)0x1F8001CF) = 0;
            (CURRENT_TASK)->unk5E = 0x78U;
            return;
        case 4:
            drawDebugText(96, 96, 0, D_80010184);
            (CURRENT_TASK)->unk5E--;
            if (((CURRENT_TASK)->unk5E << 0x10) == 0) {
                var_a0 = 1;
                temp1 = (u_char*)&GAME.areaTransition;
                if (*temp1 == 0) {
                   *temp1 = 1;
                    func_8001CE80(var_a0);
                } else {
                    if (GAME.selectedArea == GAME.currentArea) {
                        var_a0 = 0;
                        if (GAME.selectedSection == GAME.currentSection) {
                            setAreaModeFromSection(0);
                            return;
                        }
                    }
                    func_8001CE80(var_a0);
                }
                LOADING_SCREEN_STATE = 0;
                (CURRENT_TASK)->step.value++;
            }
            break;
        case 2:
        case 5:
            displayLoadingScreen();
            break;
    }
    return;
}
#else
#define SKIP_ASM
#include "common.h"
#include "game.h"

void debugSelectHandler(void)
{
    int var_a0;
    Task* p;
    u_char* temp1;
    u_char* temp2;

    switch ((CURRENT_TASK)->step.value) {
        case 0:
            func_800222B8(9, 1);
            (CURRENT_TASK)->step.value++;
            return;
        case 1:
            if (LOAD_COMPLETE != 0) {
                (CURRENT_TASK)->step.value++;
                return;
            }
        default:
            return;
        case 2:
            (CURRENT_TASK)->step.value++;
            initObjectPools();
            *(char* )0x1F8001CF = 0;
            return;
        case 3:
            displayDebugScreen();
            return;
        case 5:
            func_80020FAC();
            (*(Task**)(&SCRATCHPAD+0x1D4))->step.value++;
            initObjectPools();
            *(u_char*)&(*(u_long**)0x1F8001CF) = 0;
            (CURRENT_TASK)->unk5E = 0x78U;
            (CURRENT_TASK)->unk64 = 0U;
            return;
        case 6:
            p = *(Task**)(&SCRATCHPAD+0x1D4);
            *(u_short*)&p->unk64=((p->unk64+12)&0xFF);
            drawNowLoading(p->unk64);
            (CURRENT_TASK)->unk5E--;
            if (((CURRENT_TASK)->unk5E << 0x10) == 0) {
                var_a0 = 1;
                temp1 = (u_char*)&GAME.areaTransition;
                if (*temp1 == 0) {
                   *temp1 = 1;
                    func_8001CE80(var_a0);
                } else {
                    if (GAME.selectedArea == GAME.currentArea) {
                        var_a0 = 0;
                        if (GAME.selectedSection == GAME.currentSection) {
                            setAreaModeFromSection(0);
                            return;
                        }
                    }
                    func_8001CE80(var_a0);
                }
                LOADING_SCREEN_STATE = 0;
                (CURRENT_TASK)->step.value++;
            }
            break;
        case 4:
        case 7:
            displayLoadingScreen();
            break;
    }
    return;
}
#endif
