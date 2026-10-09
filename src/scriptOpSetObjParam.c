// FUNC 8003adf4 80 MAIN0
// MATCHING 8003adf4 80
// Ported from psx_tomba (scriptop.c, scriptOpSetObjParam); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpSetObjParam(void)
{
    ScriptContext* p = SCRIPT_CTX;
    u8* obj = SCRIPT_OBJECTS[*(s32*)((u8*)p + 0x1190)];

    if (obj != NULL) {
        *(u8*)(obj + 0x9C) = *(s32*)((u8*)p + 0x1194);
    }
    p->pc++;
}
