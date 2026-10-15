// VERSION SLPM_80165 slpm80165 800385A4 80
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8003A2D8 (scriptOpSetObjFrame.c): byte at +0x6B (PAL halfword at +0x2E).
// Ported from psx_tomba (scriptop.c, scriptOpSetObjFrame); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpSetObjTouchFlag(void)
{
    ScriptContext* p = SCRIPT_CTX;
    u8* obj = SCRIPT_OBJECTS[*(s32*)((u8*)p + 0x1190)];

    if (obj != NULL) {
        *(u8*)(obj + 0x6B) = *(s32*)((u8*)p + 0x1194);
    }
    p->pc++;
}
#endif
