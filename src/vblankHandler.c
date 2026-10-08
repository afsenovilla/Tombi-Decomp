// FUNC 800173d8 60 MAIN0
// MATCHING 800173d8 60
// Ported from psx_tomba (task.c, vblankHandler); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void vblankHandler(void)
{
    scratchpad* scratch = PSX_SCRATCH;

    scratch->vblankCount++;
    scratch->frameCount++;
}
