// FUNC 80039d5c 92 MAIN0
// MATCHING 80039d5c 92
// Ported from psx_tomba (scriptop.c, scriptOpKillObject); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpKillObject(void)
{
    ScriptContext* p = SCRIPT_CTX;
    u8** slot = SCRIPT_OBJECTS + *(s32*)((u8*)p + 0x1190);
    u8*  obj = *slot;
    u8*  other;

    if (obj != NULL) {
        other = *(u8**)(obj + 0x94);
        obj[4] = 3;
        if (other != NULL) {
            other[4] = 3;
        }
        *slot = NULL;
    }
    p->pc++;
}
