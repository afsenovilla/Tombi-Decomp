// FUNC 8001726c 80 MAIN0
// MATCHING 8001726c 80
#if defined(VERSION_PCPX_96084) /* PrePre 8 scratchpad layout */
#define TASK_C D_800DE0B0
#endif
extern void EnterCriticalSection(void);
extern void CloseTh(int);
extern void ExitCriticalSection(void);
extern void ChangeTh(int);
extern short *TASK_C;
#define P TASK_C
void exitTask(void)
{
    *P = 0;
    EnterCriticalSection();
    CloseTh(*(int *)(P + 2));
    ExitCriticalSection();
    ChangeTh(0xff000000);
}
