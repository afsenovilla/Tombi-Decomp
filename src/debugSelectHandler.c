// FUNC 8001a43c 528 MAIN0
// MATCHING 8001a43c 528
// Portado de psx_tomba (gamestate.c, debugSelectHandler); licencia MIT del proyecto original.
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

void debugSelectHandler(void)
{
    int var_a0;
    unkstruct_1F8001D4* p;
    u_char* temp1;
    u_char* temp2;

    switch ((CURRENT_TASK)->unk4E.value) {
        case 0:
            func_800222B8(9, 1);
            (CURRENT_TASK)->unk4E.value++;
            return;
        case 1:
            if (LOAD_COMPLETE != 0) {
                (CURRENT_TASK)->unk4E.value++;
                return;
            }
        default:
            return;
        case 2:
            (CURRENT_TASK)->unk4E.value++;
            initObjectPools();
            *(char* )0x1F8001CF = 0;
            return;
        case 3:
            displayDebugScreen();
            return;
        case 5:
            func_80020FAC();
            (*(unkstruct_1F8001D4**)(&SCRATCHPAD+0x1D4))->unk4E.value++;
            initObjectPools();
            *(u_char*)&(*(u_long**)0x1F8001CF) = 0;
            (CURRENT_TASK)->unk5E = 0x78U;
            (CURRENT_TASK)->unk64 = 0U;
            return;
        case 6:
            p = *(unkstruct_1F8001D4**)(&SCRATCHPAD+0x1D4);
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
                            setAreaSubState(0);
                            return;
                        }
                    }
                    func_8001CE80(var_a0);
                }
                D_8009EB4C = 0;
                (CURRENT_TASK)->unk4E.value++;
            }
            break;
        case 4:
        case 7:
            displayLoadingScreen();
            break;
    }
    return;
}
