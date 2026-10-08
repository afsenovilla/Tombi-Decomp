// FUNC 8003a3d0 80 MAIN0
// MATCHING 8003a3d0 80
// Ported from psx_tomba (scriptop.c, scriptOpSetObjEnabled); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpSetObjEnabled(void)
{
    unkstruct_8009E458* p = D_8009E458;
    u8* obj = D_8009E640[*(s32*)((u8*)p + 0x1190)];

    if (obj != NULL) {
        *(u8*)(obj + 0x0) = *(s32*)((u8*)p + 0x1194);
    }
    p->pc++;
}
