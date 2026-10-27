// FUNC 8004d20c 84 MAIN0
// MATCHING 8004d20c 84
// Ported from psx_tomba (objpool.c, initSectionSpawnerAlt); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void initSectionSpawnerAlt(u8* self)
{
    if (self[4] == 0) {
        *(s16*)(self + 0xC) = GAME.selectedArea;
        self[0xE] = GAME_SELECTED_SECTION;
        self[4] = self[4] + 1;
        func_8005B1F8(self);
    }
}
