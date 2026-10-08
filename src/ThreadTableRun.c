// FUNC 80017088 212 MAIN0
// MATCHING 80017088 212
extern unsigned short *DAT_1f8001d4;
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern int OpenTh(int, int, int);
extern void ChangeTh(int);
void ThreadTableRun(void)
{
    unsigned short *p;
    DAT_1f8001d4 = (unsigned short *)0x801fd800;
    do {
        if (*DAT_1f8001d4 != 2) {
            if (*DAT_1f8001d4 != 3) goto next;
            EnterCriticalSection();
            *(int *)(DAT_1f8001d4 + 2) = OpenTh(*(int *)(DAT_1f8001d4 + 6), *(int *)(DAT_1f8001d4 + 4), *(int *)(DAT_1f8001d4 + 8));
            ExitCriticalSection();
        }
        p = DAT_1f8001d4;
        *p = 4;
        ChangeTh(*(int *)(p + 2));
    next:
        DAT_1f8001d4 = DAT_1f8001d4 + 0x38;
    } while (DAT_1f8001d4 <= (unsigned short *)0x801fd94f);
}
