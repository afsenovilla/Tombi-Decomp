// VERSION SLPM_80165 slpm80165 8005D9F0 56
#if defined(VERSION_SLPM_80165)
// Demo version: JP C of src/_padStopCom.c (func_80064DE4) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
extern char D_8009BF78[];
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern void ChangeClearRCnt();
extern void SysDeqIntRP(int, void *);

int func_8005D9F0_slpm80165(void)
{
    EnterCriticalSection();
    SysDeqIntRP(1, D_8009BF78);
    ExitCriticalSection();
    return 1;
    ChangeClearRCnt();
}
#endif
