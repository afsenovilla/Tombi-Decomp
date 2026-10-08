// FUNC 800171b8 128 MAIN0
// MATCHING 800171b8 128
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern int OpenTh(int, int, int);
void ThreadCreate(int n, int fn)
{
    int k = n * 0x70;
    *(short *)(k + 0x801fd800) = 2;
    EnterCriticalSection();
    *(int *)(k + 0x801fd804) = OpenTh(fn, *(int *)(k + 0x801fd808), *(int *)(k + 0x801fd810));
    ExitCriticalSection();
}
