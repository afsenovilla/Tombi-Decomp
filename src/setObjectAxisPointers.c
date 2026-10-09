// FUNC 800207ec 60 MAIN0
// MATCHING 800207ec 60
// Ported from psx_tomba (entity.c, setObjectAxisPointers); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void setObjectAxisPointers(u8* self)
{
    if ((*(u16*)0x1F8001C8 & 1) == 0) {
        *(u8**)(self + 0x40) = self + 0x10;
        *(u8**)(self + 0x44) = self + 0x18;
    } else {
        *(u8**)(self + 0x44) = self + 0x10;
        *(u8**)(self + 0x40) = self + 0x18;
    }
}
