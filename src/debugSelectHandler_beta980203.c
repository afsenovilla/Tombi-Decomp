// VERSION BETA_980203 beta980203 8001AB4C 524
#if defined(VERSION_BETA_980203)
// Beta version: PAL debugSelectHandler.c (psx_tomba gamestate.c) with the case-0 load call taking one argument (PAL func_800222B8(9, 1)).
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
            func_800222B8(9);
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
