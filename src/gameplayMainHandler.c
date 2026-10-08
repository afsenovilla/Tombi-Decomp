// FUNC 8001a64c 756 MAIN0
// MATCHING 8001a64c 756
// Portado de psx_tomba (gamestate.c, gameplayMainHandler); licencia MIT del proyecto original.
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

void gameplayMainHandler(void)
{
    u_short temp_v1;
    u_char temp_v0;
    unkstruct_1F8001D4* temp_a0;
    unkstruct_1F8001D4* temp_a0_2;
    unkstruct_1F8001D4* temp_v1_2;

    temp_a0 = CURRENT_TASK;
    temp_v1 = temp_a0->unk4E.value;
    switch (temp_v1) {                              // irregular
        case 0:
            initObjectPools(temp_a0);
            initHud();
            temp_a0_2 = *(unkstruct_1F8001D4** )(&SCRATCHPAD+0x1D4);
            *(char* )0x1F8001CF = 1;
            temp_a0_2->unk4E.value++;
            func_800243E8();
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
            *(&D_8009C9D8) = D_8009C9DC = 0;
            *(short*)(&SCRATCHPAD+0x1FC)=0;
            return;
        case 1:
            GAME.totalTimePlayed++;
            gameplayTick(temp_a0);
            if (D_8009BCA0 == 2) {
                temp_v1_2 = *(unkstruct_1F8001D4** )(&SCRATCHPAD+0x1D4);
                temp_v1_2->unk4E.value++;
                func_80020C00(1);
            }
            if ((*(u_char* )0x1F8001C2 != 0) && (*&D_8009C9D8 & 8) && (*&D_8009C9D8 & 0x800)) {
                (*(unkstruct_1F8001D4** )(&SCRATCHPAD+0x1D4))->unk4E.value = 3U;
                return;
            }
            return;
        case 2:
            if ((*(u_char* )0x1F8001BB == 0) && (temp_v0 = GAME.playerLives - 1, GAME.playerLives = temp_v0, ((temp_v0 & 0xFF) == 0))) {
                temp_a0->unk4E.value = 3U;
            } else {
                temp_a0->state2 = 0;
                temp_a0->unk4E.value = 5U;
                GAME.selectedSpawnPoint = 0;
                GAME.nextSpawnPoint = 0;
                GAME.playerHealth = GAME.playerHealthDisplayed;
            }
            GAME.displayExpBar = 0;
            D_800B07CD = 0;
            GAME.keepBgm = 0;
            D_8009D6DD = 0;
            D_8009D6DE = 0;
            D_8009D6DF = 0;
            D_8009E3ED = 0;
            D_8009E3EE = 0;
            D_8009E3EF = 0;
            GAME.currentArea = GAME.selectedArea;
            GAME.currentSection = GAME.selectedSection;
            GAME.currentSpawnPoint = GAME.selectedSpawnPoint;
            return;
        case 3:
            D_8009D6DD = 0;
            D_8009D6DE = 0;
            D_8009D6DF = 0;
            D_8009E3ED = 0;
            D_8009E3EE = 0;
            D_8009E3EF = 0;
            temp_a0->state2 = 8;
            temp_a0->unk4E.value = 0U;
            break;
    }
}
