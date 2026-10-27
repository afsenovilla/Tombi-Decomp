// FUNC 8003ac94 68 MAIN0
// MATCHING 8003ac94 68
// Ported from psx_tomba (scriptop.c, scriptOpSetFadeEffect); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpSetFadeEffect(void)
{
    ScriptContext* p = SCRIPT_CTX;
    s32 a = *(s32*)((u8*)p + 0x1190);
    s32 b = *(s32*)((u8*)p + 0x1194);

    GAME_FADE_SCREEN_CONTROL = 0x10;
    D_8009BCA4 = a;
    GAME_FADE_SCREEN_AMOUNT = b;
    asm("");
    p->pc++;
}
