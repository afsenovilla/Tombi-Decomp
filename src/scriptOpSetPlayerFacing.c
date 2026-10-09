// FUNC 8003aa3c 44 MAIN0
// MATCHING 8003aa3c 44
// Ported from psx_tomba (scriptop.c, scriptOpSetPlayerFacing); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpSetPlayerFacing(void)
{
    ScriptContext* q = SCRIPT_CTX;

    D_800A53C6 = *(s32*)((u8*)q + 0x1190);
    asm("");
    q->pc++;
}
