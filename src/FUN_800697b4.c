// FUNC 800697b4 204 MAIN0
// MATCHING 800697b4 204
// FLAGS -O2 -G0
// AS gnu
/* _padStartCom (US psx_tomba libpad/padmain.c). The `frames` asm keeps the idle-frame array base in a register for both stores. */
#include "padlocal.h"

void FUN_800697b4(void)
{
    D_800981E4 = 0;
    EnterCriticalSection();
    SysDeqIntRP(2, D_8009BF78);
    SysEnqIntRP(2, D_8009BF78);
    D_8009820C->stat = ~1;
    D_8009820C->mask |= 1;
    ChangeClearRCnt(3, 0);
    ExitCriticalSection();
    D_800981B0(D_800981E0);
    D_800981B0(D_800981E0 + 1);
    {
        register int* frames = D_8009BF88;

        asm("" : "=r"(frames) : "0"(frames));
        frames[1] = 0;
        frames[0] = 0;
    }
    D_800981E4 = 1;
}
