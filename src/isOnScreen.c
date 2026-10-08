// FUNC 80020448 72 MAIN0
// MATCHING 80020448 72
// Ported from psx_tomba (entity.c, isOnScreen); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


s32 isOnScreen(s16 x, s16 y)
{
    if ((u16)(x - D_1F800176 + 0x40) < 0x1C1) {
        return (u16)(D_1F800186 - y + 0x40) < 0x171;
    }
    return 0;
}
