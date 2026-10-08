// FUNC 8003a378 88 MAIN0
// MATCHING 8003a378 88
// Ported from psx_tomba (scriptop.c, scriptOpCheckObjActive); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpCheckObjActive(void)
{
    unkstruct_8009E458* p = D_8009E458;
    u8* obj = D_8009E640[*(s32*)((u8*)p + 0x1190)];

    if (obj != NULL) {
        *(s32*)((u8*)p + 0x1190) = ((obj[4] ^ 2) == 0);
    }
    p->pc++;
}
