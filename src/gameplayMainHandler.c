// FUNC 8001a64c 756 MAIN0
// MATCHING 8001a64c 756
// Portado de psx_tomba (gamestate.c, gameplayMainHandler); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "game.h"

void gameplayMainHandler(void)
{
    u_short temp_v1;
    u_char temp_v0;
    Task* temp_a0;
    Task* temp_a0_2;
    Task* temp_v1_2;

    temp_a0 = CURRENT_TASK;
    temp_v1 = temp_a0->step.value;
    switch (temp_v1) {                              // irregular
        case 0:
            initObjectPools(temp_a0);
            initHud();
            temp_a0_2 = *(Task** )(&SCRATCHPAD+0x1D4);
            *(char* )0x1F8001CF = 1;
            temp_a0_2->step.value++;
            initPlayerAtSpawn();
            func_800246B0();
            if (*(u_long*)&GAME.selectedArea == AREA06_DIRTMOTOCROSS) {
                func_8011AF40();
            } else {
                func_80028EF4();
            }
            func_80059F7C();
            if (GAME.keepBgm != 1) {
                startAreaBgm();
            }
            GAME.unk21 = 1;
            *(&PAD_PRESSED) = D_8009C9DC = 0;
            *(short*)(&SCRATCHPAD+0x1FC)=0;
            return;
        case 1:
            GAME.totalTimePlayed++;
            gameplayTick(temp_a0);
            if (GVARS_PLAYER_HURT_STATE == 2) {
                temp_v1_2 = *(Task** )(&SCRATCHPAD+0x1D4);
                temp_v1_2->step.value++;
                func_80020C00(1);
            }
            if ((*(u_char* )0x1F8001C2 != 0) && (*&PAD_PRESSED & 8) && (*&PAD_PRESSED & 0x800)) {
                (*(Task** )(&SCRATCHPAD+0x1D4))->step.value = 3U;
                return;
            }
            return;
        case 2:
            if ((*(u_char* )0x1F8001BB == 0) && (temp_v0 = GAME.playerLives - 1, GAME.playerLives = temp_v0, ((temp_v0 & 0xFF) == 0))) {
                temp_a0->step.value = 3U;
            } else {
                temp_a0->state2 = 0;
                temp_a0->step.value = 5U;
                GAME.selectedSpawnPoint = 0;
                GAME.nextSpawnPoint = 0;
                GAME.playerHealth = GAME.playerMaxHealth;
            }
            GAME.displayExpBar = 0;
            D_800B07CD = 0;
            GAME.keepBgm = 0;
            DISPLAY_BUFFER_0_BG_R = 0;
            DISPLAY_BUFFER_0_BG_G = 0;
            DISPLAY_BUFFER_0_BG_B = 0;
            DISPLAY_BUFFER_1_BG_R = 0;
            DISPLAY_BUFFER_1_BG_G = 0;
            DISPLAY_BUFFER_1_BG_B = 0;
            GAME.currentArea = GAME.selectedArea;
            GAME.currentSection = GAME.selectedSection;
            GAME.currentSpawnPoint = GAME.selectedSpawnPoint;
            return;
        case 3:
            DISPLAY_BUFFER_0_BG_R = 0;
            DISPLAY_BUFFER_0_BG_G = 0;
            DISPLAY_BUFFER_0_BG_B = 0;
            DISPLAY_BUFFER_1_BG_R = 0;
            DISPLAY_BUFFER_1_BG_G = 0;
            DISPLAY_BUFFER_1_BG_B = 0;
            temp_a0->state2 = 8;
            temp_a0->step.value = 0U;
            break;
    }
}
