// FUNC 80033188 60 MAIN0
// MATCHING 80033188 60
// Ported from psx_tomba (objlogic.c, func_800365DC); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void func_800365DC(u8* self)
{
    if (self[6] == 0) {
        *(s16*)(self + 0x22) = 0;
        func_800384F0(self, D_8009C61A - 5);
    }
}
