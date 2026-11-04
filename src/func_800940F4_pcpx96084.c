// VERSION PCPX_96084 tomba 800940F4 56
#if defined(VERSION_PCPX_96084)
// PrePre 8 version: JP C of src/_padStopCom.c (func_80064DE4) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
extern char D_8009BF78[];
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern void ChangeClearRCnt();
extern void SysDeqIntRP(int, void *);

int func_800940F4_pcpx96084(void)
{
    EnterCriticalSection();
    SysDeqIntRP(1, D_8009BF78);
    ExitCriticalSection();
    return 1;
    ChangeClearRCnt();
}
#endif
