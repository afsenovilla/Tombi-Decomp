// FUNC 8003a1f4 120 MAIN0
// MATCHING 8003a1f4 120
// Ported from psx_tomba (scriptop.c, scriptOpGetObjPosition); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpGetObjPosition(void)
{
    unkstruct_8009E458* p = D_8009E458;
    u8* obj = D_8009E640[*(s32*)((u8*)p + 0x1190)];

    if (obj != NULL) {
        *(s32*)((u8*)p + 0x1190) = *(s16*)(*(u8**)(obj + 0x40) + 2);
        *(s32*)((u8*)p + 0x1194) = *(s16*)(obj + 0x16);
        *(s32*)((u8*)p + 0x1198) = *(s16*)(*(u8**)(obj + 0x44) + 2);
    }
    p->pc++;
}
