// VERSION PCPX_96084 tomba 80017818 128
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL main0 800171B8 (src/ThreadCreate.c): the task table is at 0x801FC000 (literal addresses).
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern int OpenTh(int, int, int);

void ThreadCreate(int n, int fn)
{
    int k = n * 0x70;
    *(short *)(k + 0x801fc000) = 2;
    EnterCriticalSection();
    *(int *)(k + 0x801fc004) = OpenTh(fn, *(int *)(k + 0x801fc008), *(int *)(k + 0x801fc010));
    ExitCriticalSection();
}
#endif
