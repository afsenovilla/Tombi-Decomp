// FUNC 8003a2d8 80 MAIN0
// MATCHING 8003a2d8 80
// Ported from psx_tomba (scriptop.c, scriptOpSetObjFrame); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpSetObjFrame(void)
{
    unkstruct_8009E458* p = D_8009E458;
    u8* obj = D_8009E640[*(s32*)((u8*)p + 0x1190)];

    if (obj != NULL) {
        *(s16*)(obj + 0x2E) = *(s32*)((u8*)p + 0x1194);
    }
    p->pc++;
}
