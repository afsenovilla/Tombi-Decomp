// FUNC 80038238 88 MAIN0
// MATCHING 80038238 88
// Ported from psx_tomba (script.c, scriptOpSwapVars); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpSwapVars(void)
{
    ScriptContext* p = SCRIPT_CTX;
    u8*  q = (u8*)(p->pc + (s32)SCRIPT_CODE);
    s32* a = (s32*)(q[1] * 4 + (s32)p + 0x1090);
    s32* b = (s32*)(q[2] * 4 + (s32)p + 0x1090);
    s32  x = *b;
    s32  y = *a;

    *a = x;
    *b = y;
    p->pc += 3;
}
