// FUNC 8003ae44 80 MAIN0
// MATCHING 8003ae44 80
// Ported from psx_tomba (scriptop.c, scriptOpMoveCamera); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpMoveCamera(void)
{
    ScriptContext* temp_s0;

    temp_s0 = SCRIPT_CTX;
    func_800EDE44(PLAYER, *(s16*)&*(s32*)((u8*)temp_s0 + 0x1190), *(s16*)&*(s32*)((u8*)temp_s0 + 0x1194));
    temp_s0->pc++;
}
