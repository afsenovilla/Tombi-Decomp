// FUNC 8001a0f0 844 MAIN0
// MATCHING 8001a0f0 844
// Portado de psx_tomba (gamestate.c, displayDebugScreen); licencia MIT del proyecto original.
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

void displayDebugScreen(void)
{
    scratchpad* scratch = PSX_SCRATCH;
    unkstruct_1F8001D4* temp_v1 = *(unkstruct_1F8001D4**)&scratch->currentTask;
    u_short var_a0;
    int* var_v1;
    u_short temp_a0;
    u_long temp_v1_2;
    u_char* temp_v1_3;
    u_char *temp2;

    /* If start a game and debug mode is enabled */
    if ((temp_v1->loadGameSelected == 0) && (scratch->debug_mode_enabled != 0)) {
        // If button UP is pressed decrease selected row
        if (scratch->joypad_state & JOY_UP) {
            D_8009B6A8 = (D_8009B6A8 - 1) & 1;
        }
        // If button DOWN is pressed increase selected row
        if (scratch->joypad_state & JOY_DOWN) {
            D_8009B6A8 = (D_8009B6A8 + 1) & 1;
        }
        // If selected row is the second one "SELECTED SECTION"
        if (D_8009B6A8 != 0) {
            // If button LEFT is pressed decrease selected section
            if (scratch->joypad_state & JOY_LEFT) {
                var_v1 = &GAME.selectedSection;
                *(u_short*)var_v1 -= 1;
                /* If selected section is less than the min section allowed for the current area,
                   clamp it to the min section */
                if ((*(u_short*)var_v1 << 0x10) <= 0) {
                    *(u_short*)var_v1 = 0U;
                }
            // If button RIGHT is pressed increase selected section
            } else if (scratch->joypad_state & JOY_RIGHT) {
                temp_v1_2 = GAME.selectedSection += 1;
                /* If selected section is greater than the max section allowed for the current area,
                   clamp it to the max section */
                temp_a0 = *(u_short*)((u_short*)&D_8007B294 + GAME.selectedArea);
                if ((temp_a0 - 1) < (int)temp_v1_2) {
                    GAME.selectedSection = (u_short) (temp_a0 - 1);
                }
            }
        // If selected row is the first one "SELECTED AREA"
        } else {
            // If button LEFT is pressed decrease selected area option
            if (scratch->joypad_state & JOY_LEFT) {
                var_v1 = &GAME.selectedArea;
                *(u_short*)var_v1 -= 1;
                /* If selected area is less than the min area allowed,
                   clamp it to the min area */
                if ((*(u_short*)var_v1 << 0x10) <= 0) {
                    *(u_short*)var_v1 = 0U;
                }
            // If button RIGHT is pressed increase selected area option
            } else if (scratch->joypad_state & JOY_RIGHT) {
                GAME.selectedArea++;
                temp_v1_2 = (u_short*)D_8007B290;
                /* If selected area is greater than the max area allowed,
                   clamp it to the max area */
                if (temp_v1_2 < GAME.selectedArea) {
                    GAME.selectedArea = temp_v1_2;
                }
            }
        }
        // Print rows with current selected options
        sprintf(&SPRINTF_BUFFER_MSG, "AREA SELECT = %02d", GAME.selectedArea);
        fontDebugPrintf(32, 96, 0U, &SPRINTF_BUFFER_MSG);
        sprintf(&SPRINTF_BUFFER_MSG, "SECTION SELECT = %02d", GAME.selectedSection);
        fontDebugPrintf(32, 104, 0U, &SPRINTF_BUFFER_MSG);
        // Print asterisk cursor on the selected row
        sprintf(&SPRINTF_BUFFER_MSG, "*");
        fontDebugPrintf(24, ((short) D_8009B6A8 + 0xC) * 8, (u_long) (*(u_short*)&PSX_SCRATCH[0x1F6] & 0xC) >> 2, &SPRINTF_BUFFER_MSG);
        // Set next area, section and spawn point to the selected ones
        GAME.nextArea = GAME.selectedArea;
        GAME.nextSection = GAME.selectedSection;
        GAME.nextSpawnPoint = GAME.selectedSpawnPoint;
        // If any action button (CIRCLE or START) is pressed
        if (scratch->joypad_state & (JOY_CIRCLE | JOY_START)) {
            // Handle area and section exceptions cases
            /* If selected area is not VILLAGE OF ALL BEGINNINGS or DWARF FOREST
               and selected section is not VILLAGE OF ALL BEGINNINGS or FOREST OF 100 FLOWERS */
            if (
                (
                    GAME.selectedArea < AREA02_DWARFVILLAGE) &&
                    (GAME.selectedSection != (
                        AREA00_SECTION00_VILLAGEOFALLBEGINNINGS |
                        AREA01_SECTION00_FORESTOF100FLOWERS
                    )
                )
            ) {
                GAME.unk21 = 1; // Set unk21 to 1 (unknown purpose)
            }
            /* If selected area is not the VILLAGE OF ALL BEGINNINGS 
               and selected section is not VILLAGE OF ALL BEGINNINGS */
            if (*(u_long*)&GAME.selectedArea != (AREA00_VILLAGEOFALLBEGINNINGS << 16 | AREA00_SECTION00_VILLAGEOFALLBEGINNINGS)) {
                GAME.event[EVENT_CLEARTHEFOG] = 0xFF; // Set event CLEARTHEFOG to CLEARED
                GAME.playerState = 1; // Set player state to NORMAL
            }
        } else return;
    }

    temp2 = (u_char*)(&GAME.areaTransition);
    var_a0 = 1;
    if (*temp2 == 0) {
        *temp2 = 1;
    } else if (GAME.selectedArea != GAME.currentArea) {
        var_a0 = 1;
    } else {
        var_a0 = 0;
        if (GAME.selectedSection == GAME.currentSection) {
            setAreaSubState();
            return;
        }
    }
    func_8001CE80(var_a0);

    temp_v1_3 = (*(unkstruct_1F8001D4**)(&PSX_SCRATCH[0x1D4]))->unk4E.value;
    *(u_long*)&D_8009EB4C = 0;
    (*(unkstruct_1F8001D4**)(&PSX_SCRATCH[0x1D4]))->unk4E.value=temp_v1_3+1;
}
