// FUNC 80069880 68 MAIN0
// MATCHING 80069880 68
extern char D_8009BF78[];
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern void ChangeClearRCnt(int, int);
extern void SysDeqIntRP(int, void *);

void func_80069880(void)
{
    EnterCriticalSection();
    ChangeClearRCnt(3, 1);
    SysDeqIntRP(2, D_8009BF78);
    ExitCriticalSection();
}
