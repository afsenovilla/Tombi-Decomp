// FUNC 8003af94 32 MAIN0
// MATCHING 8003af94 32
// Ported from psx_tomba (scriptop.c, opNop); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void opNop(void)
{
    D_8009E458->pc++;
}
