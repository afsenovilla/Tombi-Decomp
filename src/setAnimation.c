// FUNC 80049ca4 56 MAIN0
// MATCHING 80049ca4 56
// Ported from psx_tomba (objpool.c, setAnimation); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void setAnimation(u8* self, s16 arg1)
{
    *(s16*)(self + 0xAC) = arg1;
    *(s32*)(self + 0x24) =
        *(s32*)(*(u8**)(self + 0xA8) + arg1 * 4);
    readAnimFrameCount(self);
}
