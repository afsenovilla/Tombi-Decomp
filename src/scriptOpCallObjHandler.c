// FUNC 8003a420 120 MAIN0
// MATCHING 8003a420 120
// Ported from psx_tomba (scriptop.c, scriptOpCallObjHandler); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpCallObjHandler(void)
{
    unkstruct_8009E458* p = D_8009E458;
    u8* obj = D_8009E640[*(s32*)((u8*)p + 0x1190)];

    if (obj != NULL) {
        func_80022E44(obj);
        *(s32*)((u8*)p + 0x1190) = obj[1];
    }
    p->pc++;
}
