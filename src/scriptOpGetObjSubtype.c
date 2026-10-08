// FUNC 8003a4fc 116 MAIN0
// MATCHING 8003a4fc 116
// Ported from psx_tomba (scriptop.c, scriptOpGetObjSubtype); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpGetObjSubtype(void)
{
    unkstruct_8009E458* p = D_8009E458;
    u8* obj = D_8009E640[*(s32*)((u8*)p + 0x1190)];
    u8  k;

    if (obj != NULL) {
        k = obj[2] & 0x7F;
        switch (k) {
        case 0x18:
            *(s32*)((u8*)p + 0x1190) = obj[0x68];
            break;
        case 0x19:
            *(s32*)((u8*)p + 0x1190) = obj[0x68];
            break;
        }
    } else {
        *(s32*)((u8*)p + 0x1190) = 0;
    }
    p->pc++;
}
