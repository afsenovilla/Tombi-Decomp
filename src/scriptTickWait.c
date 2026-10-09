// FUNC 80038150 52 MAIN0
// MATCHING 80038150 52
// Ported from psx_tomba (script.c, scriptTickWait); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptTickWait(void)
{
    ScriptContext* p = SCRIPT_CTX;
    u32 n = *(u32*)((u8*)p + 0x11D0) + 1;

    *(u32*)((u8*)p + 0x11D0) = n;
    if (n >= *(u32*)((u8*)p + 0x11D4)) {
        p->state = 1;
    }
}
