// FUNC 80069880 68 MAIN0
// MATCHING 80069880 68
// VERSION SLPS_01144 slps01144 80064DE4 56
#if defined(VERSION_SLPS_01144)
#if defined(VERSION_SLPS_01144)
extern char D_8009BF78[];
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern void ChangeClearRCnt();
extern void SysDeqIntRP(int, void *);

int func_80064DE4(void)
{
    EnterCriticalSection();
    SysDeqIntRP(1, D_8009BF78);
    ExitCriticalSection();
    return 1;
    ChangeClearRCnt();
}
#endif
#else
extern char D_8009BF78[];
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern void ChangeClearRCnt(int, int);
extern void SysDeqIntRP(int, void *);

void _padStopCom(void)
{
    EnterCriticalSection();
    ChangeClearRCnt(3, 1);
    SysDeqIntRP(2, D_8009BF78);
    ExitCriticalSection();
}
#endif
