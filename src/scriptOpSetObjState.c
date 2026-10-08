// FUNC 8003ada4 80 MAIN0
// MATCHING 8003ada4 80
// Ported from psx_tomba (scriptop.c, scriptOpSetObjState); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpSetObjState(void)
{
    unkstruct_8009E458* p = D_8009E458;
    u8* obj = D_8009E640[*(s32*)((u8*)p + 0x1190)];

    if (obj != NULL) {
        *(u8*)(obj + 0x69) = *(s32*)((u8*)p + 0x1194);
    }
    p->pc++;
}
