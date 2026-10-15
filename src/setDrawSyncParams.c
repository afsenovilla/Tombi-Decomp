// FUNC 80016a44 24 MAIN0
// MATCHING 80016a44 24
// Ported from psx_tomba (main.c, setDrawSyncParams); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"

//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/main", initDisplay);

void setDrawSyncParams(short id, short arg1)
{
    scratchpad* scratch = PSX_SCRATCH;

    scratch->unk1EA = id;
    scratch->useDrawSync = arg1;
}
