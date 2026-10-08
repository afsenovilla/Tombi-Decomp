// FUNC 80033568 108 MAIN0
// MATCHING 80033568 108
// Ported from psx_tomba (objlogic.c, isPlayerInRange); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


s32 isPlayerInRange(u8* self, s16 arg1, s16 arg2)
{
    u16 dx;
    u16 dy;

    dx = arg1 + (*(u16*)((u8*)D_800A53D8 + 2) - *(u16*)(*(u8**)(self + 0x40) + 2));
    if ((s32)dx <= arg1 * 2) {
        dy = arg2 + (D_800A53AE - *(u16*)(self + 0x16));
        return (s32)dy <= arg2 * 2;
    }
    return 0;
}
