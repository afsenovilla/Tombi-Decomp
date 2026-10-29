// FUNC 800172bc 108 MAIN0
// MATCHING 800172bc 108
// VERSION SLPS_01144 slps01144 80016844 108
// Ported from psx_tomba (task.c, closeTask); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


#if defined(VERSION_PCPX_96084)
extern s32 D_801FC004[];
#endif
void closeTask(s32 id)
{
    s32  off;
    u16* flag;

    off  = id * sizeof(Task);
    flag = (u16*)(TASK_TABLE + off);

    if (*flag != 0) {
        *flag = 0;
        EnterCriticalSection();
#if defined(VERSION_PCPX_96084) /* PrePre 8: splat symbolizes the thread-id field */
        CloseTh(D_801FC004[id * 0x1c]);
#else
        CloseTh(*(s32*)((TASK_TABLE + 0x4) + off));
#endif
        ExitCriticalSection();
    }
}
