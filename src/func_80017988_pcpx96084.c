// VERSION PCPX_96084 tomba 80017988 88
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL main0 80017328 (src/setTask.c): the task pointer is the RAM global D_800DE0B0.
extern short *D_800DE0B0;
#define TASK_C D_800DE0B0
extern void EnterCriticalSection(void);
extern void CloseTh(int);
extern void ExitCriticalSection(void);
extern void ChangeTh(int);
void setTask(int a)
{
    short *p = TASK_C;
    *p = 3;
    *(int *)(p + 6) = a;
    EnterCriticalSection();
    CloseTh(*(int *)(TASK_C + 2));
    ExitCriticalSection();
    ChangeTh(0xff000000);
}
#endif
