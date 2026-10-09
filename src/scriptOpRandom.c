// FUNC 80038290 108 MAIN0
// MATCHING 80038290 108
// Ported from psx_tomba (script.c, scriptOpRandom); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpRandom(void)
{
    ScriptContext* p = SCRIPT_CTX;
    u8* script = SCRIPT_CODE;
    s32 idx = script[p->pc + 1];

    *(s32*)(idx * 4 + (s32)p + 0x1090) = nextRandom();
    p->pc += 2;
}
