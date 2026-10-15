// FUNC 8003a5d4 116 MAIN0
// MATCHING 8003a5d4 116
// VERSION SLPM_80165 slpm80165 800384E8 100
// Ported from psx_tomba (scriptop.c, scriptOpLoadBackground); MIT licence of the original project.
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8003A5D4 (scriptOpLoadBackground.c): no D_8009C618 == 3 guard.
// Ported from psx_tomba (scriptop.c, scriptOpLoadBackground); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpLoadBackground(void)
{
    ScriptContext* p = SCRIPT_CTX;

    func_800EBD5C(PLAYER, *(s16*)((u8*)D_800A53D8 + 2), D_800A53AE);
    *(s32*)((u8*)p + 0x1190) = D_800A5400;
    p->pc++;
}
#else
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpLoadBackground(void)
{
    ScriptContext* p = SCRIPT_CTX;

    if (D_8009C618 != 3) {
        func_800EBD5C(PLAYER, *(s16*)((u8*)D_800A53D8 + 2), D_800A53AE);
    }
    *(s32*)((u8*)p + 0x1190) = D_800A5400;
    p->pc++;
}
#endif
