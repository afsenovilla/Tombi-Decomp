// FUNC 80017380 44 MAIN0
// MATCHING 80017380 44
// Ported from psx_tomba (task.c, setTaskFlag10); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void setTaskFlag10(s32 id)
{
    u16* p;

    p = (u16*)(TASK_TABLE + id * sizeof(unkstruct_1F8001D4));
    *p |= 0x10;
}
