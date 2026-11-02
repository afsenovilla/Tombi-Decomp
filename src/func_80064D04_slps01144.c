// near-twin of PAL StartCARD (tools/jp_autofix.py)
// VERSION SLPS_01144 slps01144 80064D04 48
#if defined(VERSION_SLPS_01144)
#define SKIP_ASM
#include "common.h"

void InitCARD(long val);

void func_80064D04_slps01144(void) {
    EnterCriticalSection();
    ChangeClearPAD(0);
    ExitCriticalSection();
    return;
    StartCARD2();
}

void StopCARD(void);
#endif
