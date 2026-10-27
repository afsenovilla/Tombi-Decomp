// FUNC 80033188 60 MAIN0
// MATCHING 80033188 60
// Ported from psx_tomba (objlogic.c, weapon5to7InitIfNew); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void weapon5to7InitIfNew(u8* self)
{
    if (self[6] == 0) {
        *(s16*)(self + 0x22) = 0;
        func_800384F0(self, GAME_EQUIPPED_WEAPON - 5);
    }
}
