// FUNC 8004d20c 84 MAIN0
// MATCHING 8004d20c 84
// Ported from psx_tomba (objpool.c, func_8005B1A4); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void func_8005B1A4(u8* self)
{
    if (self[4] == 0) {
        *(s16*)(self + 0xC) = GAME.selectedArea;
        self[0xE] = D_8009BCCA;
        self[4] = self[4] + 1;
        func_8005B1F8(self);
    }
}
