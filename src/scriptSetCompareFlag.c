// FUNC 80038120 48 MAIN0
// MATCHING 80038120 48
// Ported from psx_tomba (script.c, scriptSetCompareFlag); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptSetCompareFlag(s32 arg0)
{
    unkstruct_8009E458* p = D_8009E458;

    if (arg0 == 0) {
        p->cmpFlag = 0;
    } else if (arg0 >= 0) {
        p->cmpFlag = 2;
    } else {
        p->cmpFlag = 1;
    }
}
