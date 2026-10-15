// FUNC 800171b8 128 MAIN0
// MATCHING 800171b8 128
// VERSION SLPS_01144 slps01144 80016740 128
#if defined(VERSION_SLPS_01144)
#include "include_asm.h"
INCLUDE_ASM_V("slps01144/nonmatchings/slps01144", ThreadCreate);
#else
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern int OpenTh(int, int, int);
void ThreadCreate(int n, int fn)
{
    int k = n * 0x70;
    *(short *)(k + 0x801fd800) = 2;
    EnterCriticalSection();
    *(int *)(k + 0x801fd804) = OpenTh(fn, *(int *)(k + 0x801fd808), *(int *)(k + 0x801fd810));
    ExitCriticalSection();
}
#endif
