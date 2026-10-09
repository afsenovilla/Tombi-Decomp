// FUNC 8001c364 720 MAIN0
// MATCHING 8001c364 720
// Portado de psx_tomba (gamestate.c, func_8001CFCC); licencia MIT del proyecto original.
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

void func_8001CFCC(void)
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
        D_8009BCCF = 1;
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
                D_8009BCCF = 2;
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
                    if (D_8009BCCA + D_8009C0FE == 1) {
                        task->step.value = 5;
                    }
                }
            }
        }

        gp->selectedArea = gp->nextArea;
        gp->selectedSection = gp->nextSection;
        gp->selectedSpawnPoint = gp->nextSpawnPoint;

        variant = resolveAreaVariant() & 0xFF;

        val = D_80077084[GAME_A + (u16)D_8009EBA0][D_8009BCCA];
        *(short*)0x1F8001DE = 0;
        D_8009C610 = GAME_A;
        D_8009C612 = D_8009BCCA;
        D_8009C614 = D_8009BCEA;
        *(short*)0x1F8001DC = val;

        func_80021C24(GAME_A + (u16)D_8009EBA0);

        if (func_8001DE24(areaChanged) != -1) {
            D_8009BCCF = 2;
        }

        func_80021CC8(GAME_B + (u16)D_8009EBA0, D_8009BCCA, variant | areaChanged);
        func_8003C78C();
        startSoundTask();
        D_8009EB4C = 0;
        break;
    }
    case 5:
        displayLoadingScreen();
        break;
    }
}
