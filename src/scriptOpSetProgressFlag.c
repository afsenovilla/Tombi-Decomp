// FUNC 8003a648 104 MAIN0
// MATCHING 8003a648 104
// Ported from psx_tomba (scriptop.c, scriptOpSetProgressFlag); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpSetProgressFlag(void)
{
    unkstruct_8009E458* p = D_8009E458;

    if (D_8009C618 != 3) {
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
