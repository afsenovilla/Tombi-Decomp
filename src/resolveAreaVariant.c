// FUNC 8001beec 812 MAIN0
// MATCHING 8001beec 812
// Portado de psx_tomba (gamestate.c, resolveAreaVariant); licencia MIT del proyecto original.
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

s32 resolveAreaVariant(void)
{
    s32 var_a1;
    s32 var_v0;
    u16 var_v0_2;

    var_a1 = 0;
    switch (GAME.selectedArea) {
        case AREA07_DWARFFORESTPURIFIED:
            if ((u16)D_8009EBA0 != 6) {
                var_a1 = 1;
            }
            GAME.purifiedAreas |= GAME.selectedArea = AREA01_DWARFFOREST;
            D_8009EBA0 = 6;
            break;
        case AREA01_DWARFFOREST:
            if (GAME.purifiedAreas & PURIFIED_DWARFFOREST) {
                if (*(u16*)&D_8009EBA0 != 6) {
                    var_a1 = 1;
                }
                D_8009EBA0 = 6;
            } else {
            case AREA00_VILLAGEOFALLBEGINNINGS:
            case AREA06_DIRTMOTOCROSS:
            case AREA08_BACCUSLAKE:
            case AREA09_MUSHROOMVILLAGE:
            case AREA11_VILLAGEOFCIVILIZATION:
            case AREA13_PIGISLAND:
            case AREA14_EVILPIGS:
            case AREA15_UNKNOWN:
            case AREA16_VILLAGEOFCIVILIZATIONCLOCKTOWER:
            case AREA17_VILLAGEOFCIVILIZATIONIRONTOWER:
            case AREA18_VILLAGEOFCIVILIZATIONYCROSSING:
                if (*(u16*)&D_8009EBA0 != 0) {
                    var_a1 = 1;
                }
                D_8009EBA0 = 0;
            }
            break;
        case AREA19_VILLAGEOFCIVILIZATIONPURIFIED:
            if (GAME.selectedSection != AREA19_SECTION02_HIDDENVILLAGE) {
                if (*(u16*)&D_8009EBA0 != 0x11) {
                    var_a1 = 1;
                }
                D_8009EBA0 = 17;
                GAME.selectedArea = AREA02_DWARFVILLAGE;
            } else {
                if (*(u16*)&D_8009EBA0 != 0) {
                    var_a1 = 1;
                }
                D_8009EBA0 = 0;
            }
            break;
        case AREA02_DWARFVILLAGE:
            if (*(u16*)&D_8009EBA0 != 0) {
                var_a1 = 1;
            }
            D_8009EBA0 = 0;
            if ((GAME.purifiedAreas & PURIFIED_DWARFFOREST) && ((u16) GAME.selectedSection < 2U)) {
                D_8009EBA0 = 17;
                GAME.selectedArea = AREA02_DWARFVILLAGE;
            }
            break;
        case AREA03_PHOENIXMOUNTAIN:
            if (*(u16*)&D_8009EBA0 != 0) {
                var_a1 = 1;
            }
            D_8009EBA0 = 0;
            if (GAME.purifiedAreas & PURIFIED_PHOENIXMOUNTAIN) {
                var_v0 = (u16) GAME.selectedSection < 2U;
                if (var_v0 != 0) {
                    var_v0_2 = GAME.selectedSection + 4;
                    GAME.selectedSection = var_v0_2;
                }
            }
            break;
        case AREA12_HAUNTEDMANSIONPURIFIED:
            if (*(u16*)&D_8009EBA0 != 8) {
                var_a1 = 1;
            }
            GAME.purifiedAreas |= PURIFIED_HAUNTEDMANSION, GAME.selectedArea = AREA04_HAUNTEDMANSION;
            D_8009EBA0 = 8;
            break;
        case AREA04_HAUNTEDMANSION:
            if (GAME.purifiedAreas & PURIFIED_HAUNTEDMANSION) {
                if (*(u16*)&D_8009EBA0 != 8) {
                    var_a1 = 1;
                }
                D_8009EBA0 = 8;
            } else {
                if (*(u16*)&D_8009EBA0 != 0) {
                    var_a1 = 1;
                }
                D_8009EBA0 = 0;
            }
            break;
        case AREA05_BACCUSVILLAGE:
            if (*(u16*)&D_8009EBA0 != 0) {
                var_a1 = 1;
            }
            D_8009EBA0 = 0;
            if ((GAME.purifiedAreas & PURIFIED_BACCUSVILLAGE) && ((u16) GAME.selectedSection < 2U)) {
                var_v0_2 = GAME.selectedSection + 2;
                GAME.selectedSection = var_v0_2;
            }
            break;
        case AREA10_DEEPJUNGLE:
            if (*(u16*)&D_8009EBA0 != 0) {
                var_a1 = 1;
            }
            D_8009EBA0 = 0;
            if ((GAME.purifiedAreas & PURIFIED_TRICKVILLAGE) && (GAME.selectedSection == AREA10_SECTION03_TRICKVILLAGE)) {
                GAME.selectedSection = AREA10_SECTION07_TRICKVILLAGEPURIFIED;
            }
            if (GAME.purifiedAreas & PURIFIED_DEEPJUNGLE) {
                var_v0 = (u16) GAME.selectedSection < 3U;
                if (var_v0 != 0) {
                    var_v0_2 = GAME.selectedSection + 4;
                    GAME.selectedSection = var_v0_2;
                }
            }
            break;
    }
    return var_a1;
}
