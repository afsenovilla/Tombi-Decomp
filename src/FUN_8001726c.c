// FUNC 8001726c 80 MAIN0
// MATCHING 8001726c 80
extern void EnterCriticalSection(void);
extern void CloseTh(int);
extern void ExitCriticalSection(void);
extern void ChangeTh(int);
extern short *DAT_1f8001d4;
#define P DAT_1f8001d4
void FUN_8001726c(void)
{
    *P = 0;
    EnterCriticalSection();
    CloseTh(*(int *)(P + 2));
    ExitCriticalSection();
    ChangeTh(0xff000000);
}
