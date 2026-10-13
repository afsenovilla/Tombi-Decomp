// FUNC 80017088 212 MAIN0
// MATCHING 80017088 212
extern unsigned short *TASK_C;
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern int OpenTh(int, int, int);
extern void ChangeTh(int);
void ThreadTableRun(void)
{
    unsigned short *p;
    TASK_C = (unsigned short *)0x801fd800;
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
    } while (TASK_C <= (unsigned short *)0x801fd94f);
}
