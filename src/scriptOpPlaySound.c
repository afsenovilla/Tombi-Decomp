// FUNC 8003aae8 76 MAIN0
// MATCHING 8003aae8 76
// Ported from psx_tomba (scriptop.c, scriptOpPlaySound); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpPlaySound(void)
{
    ScriptContext* p = SCRIPT_CTX;

    playSFXWithNoteAndVolume(*(s32*)((u8*)p + 0x1190),
                  *(s32*)((u8*)p + 0x1194),
                  *(s32*)((u8*)p + 0x1198));
    p->pc++;
}
