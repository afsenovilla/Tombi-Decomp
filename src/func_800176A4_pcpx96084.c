// VERSION PCPX_96084 tomba 800176A4 212
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of PAL ThreadTableRun (src/ThreadTableRun.c): task table at 0x801FC000, current task in D_800DE0B0.
#define TASK_C D_800DE0B0
extern unsigned short *TASK_C;
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern int OpenTh(int, int, int);
extern void ChangeTh(int);
void func_800176A4_pcpx96084(void)
{
    unsigned short *p;
    TASK_C = (unsigned short *)0x801fc000;
    do {
        if (*TASK_C != 2) {
            if (*TASK_C != 3) goto next;
            EnterCriticalSection();
            *(int *)(TASK_C + 2) = OpenTh(*(int *)(TASK_C + 6), *(int *)(TASK_C + 4), *(int *)(TASK_C + 8));
            ExitCriticalSection();
        }
        p = TASK_C;
        *p = 4;
        ChangeTh(*(int *)(p + 2));
    next:
        TASK_C = TASK_C + 0x38;
    } while (TASK_C <= (unsigned short *)0x801fc14f);
}
#endif
