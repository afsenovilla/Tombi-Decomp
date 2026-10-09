// FUNC 8003860c 52 MAIN0
// MATCHING 8003860c 52
// Ported from psx_tomba (script.c, scriptOpReturn); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpReturn(void)
{
    ScriptContext* p = SCRIPT_CTX;

    p->sp = p->sp - 1;
    p->pc = *(u16*)((u8*)p + p->sp * 4 + 0x90);
}
