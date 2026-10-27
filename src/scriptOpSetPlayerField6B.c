// FUNC 8003a648 104 MAIN0
// MATCHING 8003a648 104
// VERSION SLPM_80165 slpm80165 8003854C 88
// Ported from psx_tomba (scriptop.c, scriptOpSetPlayerField6B); MIT licence of the original project.
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8003A648 (scriptOpSetPlayerField6B.c): no GAME_PLAYER_MODE == 3 guard.
// Ported from psx_tomba (scriptop.c, scriptOpSetPlayerField6B); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpSetPlayerField6B(void)
{
    ScriptContext* p = SCRIPT_CTX;

    {
        switch (*(s32*)((u8*)p + 0x1190)) {
        case 0:
            D_800A5403 = 0;
            break;
        case 1:
            D_800A5403 = 1;
            break;
        default:
            D_800A5403 = 2;
            break;
        }
    }
    p->pc++;
}
#else
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpSetPlayerField6B(void)
{
    ScriptContext* p = SCRIPT_CTX;

    if (GAME_PLAYER_MODE != 3) {
        switch (*(s32*)((u8*)p + 0x1190)) {
        case 0:
            D_800A5403 = 0;
            break;
        case 1:
            D_800A5403 = 1;
            break;
        default:
            D_800A5403 = 2;
            break;
        }
    }
    p->pc++;
}
#endif
