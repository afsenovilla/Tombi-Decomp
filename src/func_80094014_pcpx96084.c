// VERSION PCPX_96084 tomba 80094014 48
#if defined(VERSION_PCPX_96084)
// PrePre 8 version: JP C of src/func_80064D04_slps01144.c (func_80064D04_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
#define SKIP_ASM
#include "common.h"

void InitCARD(long val);

void func_80094014_pcpx96084(void) {
    EnterCriticalSection();
    ChangeClearPAD(0);
    ExitCriticalSection();
    return;
    StartCARD2();
}

void StopCARD(void);
#endif
