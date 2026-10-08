// FUNC 8003aa3c 44 MAIN0
// MATCHING 8003aa3c 44
// Ported from psx_tomba (scriptop.c, scriptOpSetCameraTarget); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpSetCameraTarget(void)
{
    unkstruct_8009E458* q = D_8009E458;

    D_800A53C6 = *(s32*)((u8*)q + 0x1190);
    asm("");
    q->pc++;
}
