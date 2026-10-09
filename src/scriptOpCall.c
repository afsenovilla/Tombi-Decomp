// FUNC 800385ac 96 MAIN0
// MATCHING 800385ac 96
// Ported from psx_tomba (script.c, scriptOpCall); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpCall(void)
{
    ScriptContext* p = SCRIPT_CTX;
    u8* script = SCRIPT_CODE;

    s32 v = p->pc + 2;

    *(s32*)((u8*)p + p->sp * 4 + 0x90) = v;
    p->sp = p->sp + 1;
    p->pc = *(u16*)((u8*)p + script[p->pc + 1] * 2) - 1;
}
