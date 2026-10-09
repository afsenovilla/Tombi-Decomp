// FUNC 800172bc 108 MAIN0
// MATCHING 800172bc 108
// Ported from psx_tomba (task.c, closeTask); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void closeTask(s32 id)
{
    s32  off;
    u16* flag;

    off  = id * sizeof(Task);
    flag = (u16*)(TASK_TABLE + off);

    if (*flag != 0) {
        *flag = 0;
        EnterCriticalSection();
        CloseTh(*(s32*)((TASK_TABLE + 0x4) + off));
        ExitCriticalSection();
    }
}
