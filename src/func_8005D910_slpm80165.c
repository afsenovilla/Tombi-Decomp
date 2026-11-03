// VERSION SLPM_80165 slpm80165 8005D910 48
#if defined(VERSION_SLPM_80165)
// Demo version: JP C of src/func_80064D04_slps01144.c (func_80064D04_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
#define SKIP_ASM
#include "common.h"

void InitCARD(long val);

void func_8005D910_slpm80165(void) {
    EnterCriticalSection();
    ChangeClearPAD(0);
    ExitCriticalSection();
    return;
    StartCARD2();
}

void StopCARD(void);
#endif
